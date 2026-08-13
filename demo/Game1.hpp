#pragma once

#include <math.h>

#include "../src/ISdfRenderPass.h"
#include "../src/AssetLoaderJob.h"
#include "../src/MouseState.h"
#include "../src/KeyboardState.h"
#include "../src/MouseModes.h"
#include "../src/IMouseInput.h"
#include "../src/IKeyboardInput.h"
#include "../src/TouchInput.h"
#include "../src/ITouchInput.h"
#include "../src/IFullscreenableDisplay.h"
#include "../src/RenderGraph.h"
#include "../src/Game.h"
#include "../src/RenderToSwapchainPass.h"

using namespace OSK;
using namespace OSK::IO;
using namespace OSK::ASSETS;
using namespace OSK::GRAPHICS;

class MinComputePass : public IRenderPass {

public:

	explicit MinComputePass(GdrImageUuid targetImage) : IRenderPass("Resources/Materials/Testing/min_compute.json", RenderPassType::COMPUTE) {
		AddImageDependency(RgImageDependency::ComputeTarget(targetImage, "image", "image"));
	}

	void Execute(ICommandList* cmdList) override {
		const auto windowRec = Engine::GetDisplay()->GetResolution();

		cmdList->PushMaterialConstants("time", Engine::GetCurrentTime());

		cmdList->DispatchCompute({
			(USize32)glm::ceil(windowRec.x / 16.0f),
			(USize32)glm::ceil(windowRec.y / 16.0f), 1 });
	}

	std::string_view GetName() const override {
		return "MinComputePass";
	}

};

class Render2dPass : public ISdfRenderPass {

public:

	explicit Render2dPass(GdrImageUuid targetImage) : ISdfRenderPass() {
		m_img = Engine::GetAssetManager()->Load<ASSETS::Texture>("Resources/Assets/Textures/engine_logo.json");

		auto imageDependency = RgImageDependency::ColorTarget(targetImage);
		imageDependency.clearIfTargetImage = false;
		AddImageDependency(imageDependency);
	}

	void Execute(ICommandList* cmdList) override {
		const auto& windowRec = Engine::GetDisplay()->GetResolution();
		auto* sdfRenderer = GetSdfRenderer();

		sdfRenderer->Begin(cmdList);
		sdfRenderer->SetCamera(Vector2f::Zero, windowRec.ToVector2f());

		auto drawCall = SdfDrawCall2D::Texture(m_img->GetTextureView2D());
		drawCall.transform.SetPosition(Vector2f((windowRec.x - m_img->GetSize().x) / 2.0f, (windowRec.y - m_img->GetSize().y) / 2.0f));
		drawCall.transform.SetScale(m_img->GetSize().ToVector2f());

		sdfRenderer->Draw(drawCall);

		sdfRenderer->End();
	}

	std::string_view GetName() const override {
		return "Render2dPass";
	}

private:

	AssetRef<Texture> m_img;

};


/// @brief Ejemplo de juego con funcionalidad mínima,
/// para comprobar el funcionamiento correcto del motor.
///
/// Usa renderizado y computación.
class GameMin : public OSK::IGame {

public:

	GameMin() : OSK::IGame(GAME::DefaultContentProfile::MINIMAL) {}

protected:

	/// @brief Crea la ventana, con título "OSKengine Min"
	void CreateWindow() override {
		Engine::GetDisplay()->Create({ 1800u, 900u }, "OSKengine Min");

		if (auto* mouseInput = GetMouse()) 
		{
			mouseInput->SetReturnMode(IO::MouseReturnMode::FREE);
			mouseInput->SetMotionMode(IO::MouseMotionMode::RAW);
		}
	}

	void SetupEngine() override {
		Engine::GetRenderer()->Initialize(
			"OSKengine minimal demo", 
			{}, 
			*Engine::GetDisplay(), 
			PresentMode::VSYNC_ON);
	}

	void OnCreate() override {
		IGame::OnCreate();
		Engine::GetJobSystem()->WaitForJobs<AssetLoaderJob>();

		auto renderGraph = MakeUnique<RenderGraph>(Engine::GetRenderer());
		renderGraph->SetFramebufferResolution(Engine::GetDisplay()->GetResolution());

		m_swapchainImages.Resize(Engine::GetRenderer()->GetSwapchainImagesCount());
		for (UIndex16 i = 0; i < m_swapchainImages.GetSize(); i++) {
			auto* img = Engine::GetRenderer()->_GetSwapchain()->GetImage(i);
			m_swapchainImages[i] = renderGraph->RegisterExternalImage(img);
		}

		RgImageRegisterInfo target{};
		target.numLayers = 1;
		target.dimension = GpuImageDimension::d2D;
		target.format = Format::RGBA8_UNORM;
		target.name = "Target Image";
		const auto targetUuid = renderGraph->RegisterImage(target, RgRelativeSizeImageRegisterArgs::From2D(Vector2f::One));

		renderGraph->RegisterRenderpass(MakeUnique<MinComputePass>(targetUuid),								RenderPassDependencies::Empty());
		renderGraph->RegisterRenderpass(MakeUnique<Render2dPass>(targetUuid),								RenderPassDependencies::After({ "MinCompute" }));
		renderGraph->RegisterRenderpass(MakeUnique<OSK::GRAPHICS::RenderToSwapchainPass>(targetUuid, m_swapchainImages.GetFullSpan()),	RenderPassDependencies::After({ "Render2dPass" }));

		renderGraph->Compile();

		SetRenderGraph(std::move(renderGraph));
	}

	void OnTick_BeforeEcs(TDeltaTime deltaTime) override {
		IGame::OnTick_BeforeEcs(deltaTime);

		static TDeltaTime totalTime = 0.0f;
		totalTime += deltaTime;
		m_frameCount++;
		
		if (totalTime >= 1.0f) {
			totalTime = 0.0f;
			Engine::GetLogger()->Log(IO::LogLevel::L_DEBUG, "FPS: ", m_frameCount);
			m_frameCount = 0;
		}

		if (auto* t = GetTouchs()) {
			for (const auto& touchEvent : t->GetCurrentFrameInputs()) {
				// Engine::GetLogger()->Log(IO::LogLevel::L_DEBUG, "TOUCH: ", touchEvent.position.x, "x", touchEvent.position.y);
			}
		}
	}

	void OnTick_Keyboard(const IO::IKeyboardInput& keyboard) override {
		// Exit
		if (keyboard.IsKeyDown(IO::Key::ESCAPE)) {
			this->Exit();
		}

		// Fullscreen
		if (keyboard.IsKeyStroked(IO::Key::F11)) {
			IO::IFullscreenableDisplay* display = nullptr;
			Engine::GetDisplay()->QueryInterface(OSK_IUUID(IO::IFullscreenableDisplay), (void**)&display);

			if (display) {
				display->ToggleFullscreen();
			}
		}
	}

	void OnWindowResize(const Vector2ui& res) override {
		auto* renderGraph = GetRenderGraph();

		for (UIndex16 i = 0; i < m_swapchainImages.GetSize(); i++) {
			auto* img = Engine::GetRenderer()->_GetSwapchain()->GetImage(i);
			renderGraph->RebindExternalImage(m_swapchainImages[i], img);
		}
	
		renderGraph->SetFramebufferResolution(res);
		renderGraph->RecreateRelativeImages();
	}

private:

	DynamicArray<GdrImageUuid> m_swapchainImages;
	int m_frameCount = 0;

};

include(CMakePrintHelpers)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(OSKengine_demo)
include_directories(../external/)

#
# Todos los archivos de código.
#
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADER_FILES CONFIGURE_DEPENDS "*.h*")

file(GLOB ENGINE_SOURCE_FILES CONFIGURE_DEPENDS "../src/*.cpp")
file(GLOB ENGINE_HEADER_FILES CONFIGURE_DEPENDS "../src/*.h*")

if (OSK_PLATFORM STREQUAL "OSK_ANDROID")
    add_library(OSKengine_demo SHARED ${SOURCE_FILES} ${HEADER_FILES} ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES} Game1.hpp)
else()
    add_executable(OSKengine_demo)
    target_sources(OSKengine_demo PRIVATE ${SOURCE_FILES} ${HEADER_FILES} ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES} Game1.hpp)
endif()

#
# #define de C++.
#
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(${OSK_PLATFORM})
add_compile_definitions(${EXTRA_DEFINES})
add_compile_definitions(${OSK_EXTRA})
add_compile_definitions(OSK_DEVELOPMENT) # Especial para generar .exe
if(OSK_DEBUG)
    add_compile_definitions(OSK_DEBUG)
endif()


#
# Búsqueda de dependencias.
#
if(OSK_PLATFORM STREQUAL "OSK_WINDOWS")
    find_package(glfw3          CONFIG REQUIRED)
    find_package(Vulkan REQUIRED)
    target_include_directories(OSKengine_demo PRIVATE ${Vulkan_INCLUDE_DIRS})
endif()

find_package(glm            CONFIG REQUIRED)
find_package(Freetype       CONFIG REQUIRED)
find_package(OpenAL         CONFIG REQUIRED)
find_package(unofficial-spirv-reflect  CONFIG REQUIRED)


#
# Dependencias.
#
if(OSK_PLATFORM STREQUAL "OSK_ANDROID")
    set(ALL_DEPENDENCIES vulkan native_app_glue android glm::glm-header-only json stbi_image tiny_gltf unofficial::spirv-reflect OpenAL::OpenAL Freetype::Freetype) #spirv-cross-c
else()
    if(OSK_PLATFORM STREQUAL "OSK_WINDOWS")
        set(ALL_DEPENDENCIES glm::glm-header-only json stbi_image tiny_gltf unofficial::spirv-reflect OpenAL::OpenAL Freetype::Freetype glfw ${VULKAN_SDK}/Lib/vulkan-1.lib) #spirv-cross-c
    else()
        set(ALL_DEPENDENCIES glm::glm-header-only json stbi_image tiny_gltf unofficial::spirv-reflect OpenAL::OpenAL Freetype::Freetype glfw ${VULKAN_SDK}/x86_64/lib/libvulkan.so.1) #spirv-cross-c
    endif()
endif()

target_link_libraries(OSKengine_demo PRIVATE ${ALL_DEPENDENCIES})


#
# Enlaces simbóldicos de engine_config.json y Resources, para no tener que
# estar copiándolos contínuamente.
#
#  
#
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/engine_config.json" "${CMAKE_CURRENT_BINARY_DIR}/engine_config.json" SYMBOLIC)
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_CURRENT_BINARY_DIR}/Resources" SYMBOLIC)

#
# Carpetas virtuales
#
file(GLOB SRC_ASSETS "AssetManager.*" "AssetOwningRef.*" "AssetRef.*" "AssetsMap.*" "IAsset.*" "IAssetLoader.*")
source_group("Assets (base)" FILES ${SRC_ASSETS})

file(GLOB SRC_ASSETS_IN "AudioAsset*" "CubemapTexture*" "IrradianceMap*" "Model3D*" "PreBuiltCollider*" "PreBuiltSpline3D*" "SpecularMap*" "Texture*" "AnimMeshLoader.*" "AudioLoader.*" "FontLoader.*" "Font.*" "FontInstance.*" "ModelLoader3D.*" "IGltfLoader.*" "StaticMeshLoader.*" "PreBuiltSplineLoader3D.*" "FontCharacter.*")
source_group("Assets (built-in)" FILES ${SRC_ASSETS_IN})

file(GLOB SRC_AUDIO "AudioFormat*" "Audiobuffer*" "IAudioDevice*" "IAudioSource*" "IAudioApi*")
source_group("Audio" FILES ${SRC_AUDIO})

file(GLOB SRC_AUDIO_AL "*Audio*Al*")
source_group("Audio Backends/AL" FILES ${SRC_AUDIO_AL})

file(GLOB SRC_COLL "Collider.*" "CollisionInfo.*" "DetailedCollisionInfo.*" "BroadColliderHolder.*" "NarrowColliderHolder.*")
source_group("Collision" FILES ${SRC_COLL})

file(GLOB SRC_COLL_BASE "BroadColliderHolder.*" "IBroadCollider.*" "INarrowCollider.*" "NarrowColliderHolder")
source_group("Collision/Base" FILES ${SRC_COLL_BASE})

file(GLOB SRC_COLL_BROAD "BroadCollisionDetector*" "IBroadCollisionDetector.*")
source_group("Collision/Broad" FILES ${SRC_COLL_BROAD})

file(GLOB SRC_COLL_NARROW "*DetailedCollisionDetector*")
source_group("Collision/Narrow" FILES ${SRC_COLL_NARROW})

file(GLOB SRC_COLL_COLLIDERS "AxisAlignedBoundingBox.*" "ConvexVolume.*" "SphereCollider.*")
source_group("Collision/Colliders" FILES ${SRC_COLL_COLLIDERS})

file(GLOB SRC_COLL_ALGO "Clipping.*" "*MinkowskiHull.*" "FaceProjection.*" "Gjk.*" "Sat.*" "Simplex.*")
source_group("Collision/Algorithms" FILES ${SRC_COLL_ALGO})

file(GLOB SRC_COLL_RAYS "IRayCollider.*" "Ray.*" "RayCast.*" "RayCastResult.*")
source_group("Collision/Rays" FILES ${SRC_COLL_RAYS})

file(GLOB SRC_COLL_DET "AabbAabbDetector.*" "BroadGjkCollisionDetector.*" "DetailedSphereSphereDetector.*" "GjkClippingDetector.*" "GjkEpaDetector.*" "SphereAabbBroadCollisionDetector.*" "SphereSphereBroadCollisionDetector.*" "AaBbBroadCollisionDetector.*" "BroadGjkDetector.*")
source_group("Collision/Detectors" FILES ${SRC_COLL_DET})

file(GLOB SRC_MACROS "ApiCall.*" "DefineAs.*" "DefineConstructors.*" "DefineIuuid.*" "VulkanTypedefs.*" )
source_group("Macros" FILES ${SRC_MACROS})

file(GLOB SRC_THREADS "AtomicHolder.*" "MutexHolder.*" "IJob.*" "JobCountMap.*" "JobQueue.*" "JobSystem.*" "ThreadPool.*")
source_group("Threads (base)" FILES ${SRC_THREADS})

file(GLOB SRC_THREADS_IN "AssetLoaderJob.*" "ConsumerSystemExecutionJob.*" "IteratorSystemExecutionJob.*")
source_group("Threads (built-in)" FILES ${SRC_THREADS_IN})

file(GLOB SRC_UI "IUiContainer.*" "UiAnchor.*" "UiElement.*" "uiRenderer.*")
source_group("UI (base)" FILES ${SRC_UI})

file(GLOB SRC_UI_CONT "UiBorderLayout.*" "UiCollapsibleWrapper.*" "UiFreeContainer.*" "UiHorizontalContainer.*" "UiVerticalContainer.*")
source_group("UI (containers)" FILES ${SRC_UI_CONT})

file(GLOB SRC_UI_EL "UiButton.*" "UiConsole.*" "UiDropdown.*" "UiImageView.*" "UiTextInput.*" "UiTextView.*")
source_group("UI (elements)" FILES ${SRC_UI_EL})

file(GLOB SRC_TYPES "Color.*" "CpuModel3D.*" "Frustum.*" "NumericTypes.*" "Plane.*" "Quaternion.*" "Spline3D.*" "StopWatch.*" "Transform3D.*" "Units.*" "Uuid.*" "Vector2.*" "Vector3.*" "Vector4.*" "Version.*")
source_group("Types" FILES ${SRC_TYPES})

file(GLOB SRC_UTILS "Assert.*" "EnumFlags.*" "IQueryInterface.*" "UtilMath.*" "MatrixOperations.*" "ShemaId.*" "StringOperations.*" "ToString.*")
source_group("Utils" FILES ${SRC_UTILS})

file(GLOB SRC_CORE "OSKengine.*" "Platforms.*" "Game.*" "IDebugGame.*" "DefaultElementsProfiles.*" "GameEntry.*" "GameEntryAndroid.*")
source_group("Core" FILES ${SRC_CORE})

file(GLOB SRC_EXCEP "*Exception*")
source_group("Excepciones" FILES ${SRC_EXCEP})

file(GLOB SRC_ECS "EntityComponentSystem.*" "SavedGameObjectTranslator.*" "GameObjectTranslator.*")
source_group("ECS (base)" FILES ${SRC_ECS})

file(GLOB SRC_ECS_SYST "IConsumerSystem.*" "IIteratorSystem.*" "ISystem.*" "SystemExecutionGraph.*" "SystemManager.*" "IRenderSystem.*")
source_group("ECS (base)/Systems" FILES ${SRC_ECS_SYST})

file(GLOB SRC_ECS_COMP "Component.*" "ComponentContainer.*" "ComponentManager.*" "IComponentContainer.*" "SerializableComponent.*")
source_group("ECS (base)/Components" FILES ${SRC_ECS_COMP})

file(GLOB SRC_ECS_EVN "Event.*" "EventContainer.*" "EventManager.*" "IEventContainer.*")
source_group("ECS (base)/Events" FILES ${SRC_ECS_EVN})

file(GLOB SRC_ECS_OBJ "GameObject.*" "GameObjectManager.*")
source_group("ECS (base)/Objects" FILES ${SRC_ECS_OBJ})

file(GLOB SRC_ECS_IN_SYST "CollisionSystem.*" "PhysicsResolver.*" "PhysicsSystem.*" "TransformApplierSystem.*")
source_group("ECS (built-in)/Systems" FILES ${SRC_ECS_IN_SYST})

file(GLOB SRC_ECS_IN_SYST_REND "RenderSystem2D.*" "DeferredRenderSystem.*" "GdrRenderSystem.*" "HybridRenderSystem.*" "ColliderRenderer.*" "RenderBoundsRenderer.*" "SkyboxRenderSystem.*" "GdrDeferredSystem.*" "GdrDeferredRenderSystem.*")
source_group("ECS (built-in)/Systems/Render (deprecated)" FILES ${SRC_ECS_IN_SYST_REND})

file(GLOB SRC_ECS_IN_COMP "CameraComponent*" "CollisionComponent.*" "ModelComponent3D.*" "PhysicsComponent.*" "Sprite.*" "TransformComponent*" "Gdr*Component*")
source_group("ECS (built-in)/Components" FILES ${SRC_ECS_IN_COMP})

file(GLOB SRC_ECS_IN_EVN "CollisionEvent.*")
source_group("ECS (built-in)/Events" FILES ${SRC_ECS_IN_EVN})

file(GLOB SRC_STD "Buffer.*" "Memory.*" "SharedPtr.*" "UniquePtr.*" "BitSet.*" "Concepts.*" "ConstexprBitSet.*" "DynamicArray.*" "HashMap.*" "Iterable.*" "Pair.*" "Span.*" "StaticArray.*" "IIterable.*")
source_group("STD" FILES ${SRC_STD})

file(GLOB SRC_GFX_ANIM "Animation.*" "AnimationChannel.*" "AnimationSampler.*" "AnimationSkin.*" "Animator.*" "Skeleton.*" "Bone.*")
source_group("Graphics (high level)/Animación 3D" FILES ${SRC_GFX_ANIM})

file(GLOB SRC_GFX_RG "GlobalVertexBuffer.*" "GlobalIndexBuffer.*" "RenderGraph.*" "RenderGraphArgs.*" "RenderGraphArgs.*" "RgBufferManager.*" "RgGeometryManager.*" "RgImageManager.*" "RgEntry.*")
source_group("Graphics (high level)/Render graph" FILES ${SRC_GFX_RG})

file(GLOB SRC_GFX_RG_IN "ExampleComputePass.*" "GdrDeferredPass.*" "GdrPipeline.*" "RenderToSwapchainPass.*")
source_group("Graphics (high level)/Render graph (built-in)" FILES ${SRC_GFX_RG_IN})

file(GLOB SRC_GFX_RG_RES "ImageUuid.*" ".*" "RgBufferDependency.*" "RgDependencyShaderBinding.*" "RgDependencyType.*" "RgImageDependency.*")
source_group("Graphics (high level)/Render graph/Resources" FILES ${SRC_GFX_RG_RES})

file(GLOB SRC_GFX_RG_RP "RenderPass.*" "RenderPassDependencies.*")
source_group("Graphics (high level)/Render graph/Renderpass" FILES ${SRC_GFX_RG_RP})

file(GLOB SRC_GFX_RTARGT "RenderTarget.*" "RenderTargetAttachment.*" "RenderTargetAttachmentInfo.*" "RtRenderTarget.*")
source_group("Graphics (high level)/Rendertargets" FILES ${SRC_GFX_RTARGT})

file(GLOB SRC_GFX_2D_SDF "ISdfRenderer2D.*" "SdfDrawCall2D.*" "SdfDrawCall.*" "SdfDrawCallType2D.*" "SdfShape2D.*" "SdfStringInfo.*")
source_group("Graphics (high level)/2D (SDF)" FILES ${SRC_GFX_2D_SDF})

file(GLOB SRC_GFX_2D_SDF_BLESS "SdfBindlessBatch2D.*" "SdfBindlessBufferContent2D.*" "SdfBindlessRenderer2D.*" "SdfGlobalInformationBufferContent2D.*")
source_group("Graphics (high level)/2D (SDF)/Bindless" FILES ${SRC_GFX_2D_SDF_BLESS})

file(GLOB SRC_GFX_2D_OTHER "FrameCombiner.*" "SpriteBatch.*" "SpriteRenderer.*" "TextureCoordinates.*" "PushConst2D.*")
source_group("Graphics (high level)/2D (otros)" FILES ${SRC_GFX_2D_OTHER})

file(GLOB SRC_GFX_RPASS "AnimatedGBufferPass.*" "BillboardGBufferPass.*" "GrassRenderPass.*" "PbrResolvePass.*" "DeferredGBufferPass.*" "ShadowsStaticPass.*" "DeferredPushConstants.*")
source_group("Graphics (high level)/Renderpass (deprecated)" FILES ${SRC_GFX_RPASS})

file(GLOB SRC_GFX_PBR "GBuffer.*" "Lights.*" "PbrIblConfig.*" "PbrMaterialInfo.*" "ShadowMap.*" "TaaProvider.*" "IblConfig.cpp")
source_group("Graphics (high level)/PBR" FILES ${SRC_GFX_PBR})

file(GLOB SRC_GFX_POST "SmaaPass.*" "ToneMapping.*" "ToneMappingPass.*" "IPostProcessPass.*" "HbaoPass.*" "FxaaPass.*" "BloomPass.*" "PbrResolverPass.*")
source_group("Graphics (high level)/Post-procesado (deprecated)" FILES ${SRC_GFX_POST})

file(GLOB SRC_GFX_SPASS "IDeferredResolver.*" "ShaderPassFactory.*" "StaticGBufferPass.*" "IShaderPass.*")
source_group("Graphics (high level)/Shader-pass (deprecated)" FILES ${SRC_GFX_SPASS})

file(GLOB SRC_GFX_SPASS "ISdfRenderPass.*" "ShaderPassFactory.*" "StaticGBufferPass.*" "IShaderPass.*")
source_group("Graphics (high level)/Render-pass (deprecated)" FILES ${SRC_GFX_SPASS})

file(GLOB SRC_GFX_LLR "ResourcesInFlight.*" "ResourceQueueTransfer.*" "RenderApiType.*" "PresentMode.*" "BindlessLimits.*" "IGpuObject.*" "GpuBarrierInfo.*")
source_group("Graphics (low level)" FILES ${SRC_GFX_LLR})

file(GLOB SRC_GFX_LLR_REND "Renderer.*" "IRenderer.*" "RendererVk.*")
source_group("Graphics (low level)/Renderer" FILES ${SRC_GFX_LLR_REND})

file(GLOB SRC_GFX_LLR_RP_VK "FramebufferVk.*" "FramebufferVkMap.*" "RenderpassAttachmentVk.*" "RenderpassType.*" "RenderpassVk.*" "RenderpassVkMap.*" "RenderSubpassVk.*")
source_group("Graphics (low level)/Renderpass (VK)" FILES ${SRC_GFX_LLR_RP_VK})

file(GLOB SRC_GFX_LLR_SW "ISwapchain.*" "SwapchainVk.*")
source_group("Graphics (low level)/Swapchain" FILES ${SRC_GFX_LLR_SW})

file(GLOB SRC_GFX_LLR_RT_AS_BOTTOM "IBottomLevelAccelerationStructure.*" "BottomLevelAccelerationStructureVk.*")
source_group("Graphics (low level)/Rays/Acceleration Structures (bottom)" FILES ${SRC_GFX_LLR_RT_AS_BOTTOM})

file(GLOB SRC_GFX_LLR_RT_AS_TOP "ITopLevelAccelerationStructure.*" "TopLevelAccelerationStructureVk.*")
source_group("Graphics (low level)/Rays/Acceleration Structures (top)" FILES ${SRC_GFX_LLR_RT_AS_TOP})

file(GLOB SRC_GFX_LLR_RT_AS "RtAccelerationStructureFlags.*")
source_group("Graphics (low level)/Rays/Acceleration Structures (base)" FILES ${SRC_GFX_LLR_RT_AS})

file(GLOB SRC_GFX_LLR_RT_SH "IRtShaderTable.*" "RtShaderTableVk.*")
source_group("Graphics (low level)/Rays/Shader tables" FILES ${SRC_GFX_LLR_RT_SH})

file(GLOB SRC_GFX_LLR_MEM "GpuHeapMemoryUsageInfo.*" "GpuMemoryAlignments.*" "GpuMemoryUsageInfo.*" "GpuMemoryTypes.*" "GpuMemoryTypesVk.*" "GpuMemoryTypeVk.*")
source_group("Graphics (low level)/Memory" FILES ${SRC_GFX_LLR_MEM})

file(GLOB SRC_GFX_LLR_MEM_ALC "IGpuMemoryAllocator.*" "GpuMemoryAllocatorVk.*")
source_group("Graphics (low level)/Memory/Allocator" FILES ${SRC_GFX_LLR_MEM_ALC})

file(GLOB SRC_GFX_LLR_MEM_ALC "IGpuMemoryBlock.*" "GpuMemoryBlockVk.*")
source_group("Graphics (low level)/Memory/Blocks" FILES ${SRC_GFX_LLR_MEM_ALC})

file(GLOB SRC_GFX_LLR_MEM_ALC "IGpuMemorySubblock.*" "GpuMemorySubblockVk.*")
source_group("Graphics (low level)/Memory/Subblocks" FILES ${SRC_GFX_LLR_MEM_ALC})

file(GLOB SRC_GFX_LLR_MEM_GPU "GpuInfoVkAny.*" "GpuVk.*" "IGpu.*")
source_group("Graphics (low level)/GPU" FILES ${SRC_GFX_LLR_MEM_GPU})

file(GLOB SRC_GFX_LLR_CMD_LIST "CommandListVk.*" "ICommandList.*" "CopyImageInfo.*")
source_group("Graphics (low level)/Commands (list)" FILES ${SRC_GFX_LLR_CMD_LIST})

file(GLOB SRC_GFX_LLR_CMD_POOL "CommandPoolVk.*" "ICommandPool.*" "ThreadedCommandPoolMap.*")
source_group("Graphics (low level)/Commands (pool)" FILES ${SRC_GFX_LLR_CMD_POOL})

file(GLOB SRC_GFX_LLR_CMD_Q "CommandQueueSupport.*" "CommandQueueVk.*" "GpuQueueTypes.*" "ICommandQueue.*" "QueueFamilyIndices.*" "GpuQueueTypes.*" "GpuQueueTypes.*")
source_group("Graphics (low level)/Commands (queue)" FILES ${SRC_GFX_LLR_CMD_Q})

file(GLOB SRC_GFX_LLR_CMD_B "CommandsSupport.*")
source_group("Graphics (low level)/Commands (base)" FILES ${SRC_GFX_LLR_CMD_B})

file(GLOB SRC_GFX_LLR_MAT "MaterialLayoutBuilder.*" "SpirvReflectionData.*")
source_group("Graphics (low level)/Material" FILES ${SRC_GFX_LLR_MAT})

file(GLOB SRC_GFX_LLR_MAT_SLOT "IMaterialSlot.*" "MaterialSlotVk.*")
source_group("Graphics (low level)/Material/Slot" FILES ${SRC_GFX_LLR_MAT_SLOT})

file(GLOB SRC_GFX_LLR_MAT_DESC "DescriptorLayoutVk.*" "DescriptorPoolVk.*")
source_group("Graphics (low level)/Material/Descriptors (VK)" FILES ${SRC_GFX_LLR_MAT_DESC})

file(GLOB SRC_GFX_LLR_RES_BF "GpuBuffer.*" "VertexBufferView.*" "IndexBufferView.*" "ImageBufferView.*" "GpuBufferRange.*")
source_group("Graphics (low level)/Buffers" FILES ${SRC_GFX_LLR_RES_BF})

file(GLOB SRC_GFX_LLR_RES_IMG "IGpuImage.*" "GpuImageVk.*" "GpuImageRange.*" "GpuImageCreateInfo.*" "GpuImageDimensions.*" "GpuImageUsage.*")
source_group("Graphics (low level)/Images (base)" FILES ${SRC_GFX_LLR_RES_IMG})

file(GLOB SRC_GFX_LLR_RES_IMG_FMT "Format.*" "FormatVk.*")
source_group("Graphics (low level)/Images (format)" FILES ${SRC_GFX_LLR_RES_IMG_FMT})

file(GLOB SRC_GFX_LLR_RES_IMG_LOUT "GpuImageLayout.*" "GpuImageLayoutVk.*")
source_group("Graphics (low level)/Images (layout)" FILES ${SRC_GFX_LLR_RES_IMG_LOUT})

file(GLOB SRC_GFX_LLR_RES_IMG_SPLER "GpuImageSamplerDesc.*" "GpuImageSamplerVk.*" "IGpuImageSampler.*" "GpuImageSampler.*")
source_group("Graphics (low level)/Images (sampler)" FILES ${SRC_GFX_LLR_RES_IMG_SPLER})

file(GLOB SRC_GFX_LLR_RES_IMG_VIEW "GpuImageViewConfig.*" "IGpuImageView.*" "GpuImageViewVk.*")
source_group("Graphics (low level)/Images (views)" FILES ${SRC_GFX_LLR_RES_IMG_VIEW})

file(GLOB SRC_GFX_LLR_RES_VTX "Vertex.*" "VertexAttributes.*" "IGpuVertexDescription.*" "GpuVertexDescription.*" "GpuVertexDescriptionVk.*" "VertexInfo.*" "IGpuVertexDescription")
source_group("Graphics (low level)/Vertex (base)" FILES ${SRC_GFX_LLR_RES_VTX})

file(GLOB SRC_GFX_LLR_RES_VTX_IN "Vertex2D.*" "Vertex3D.*")
source_group("Graphics (low level)/Vertex (built-in)" FILES ${SRC_GFX_LLR_RES_VTX_IN})

file(GLOB SRC_GFX_LLR_PIPE_COMP "ComputePipelineVk.*" "IComputePipeline.*")
source_group("Graphics (low level)/Pipeline/Compute" FILES ${SRC_GFX_LLR_PIPE_COMP})

file(GLOB SRC_GFX_LLR_PIPE_GFX "GraphicsPipelineVk.*" "IGraphicsPipeline.*")
source_group("Graphics (low level)/Pipeline/Graphics" FILES ${SRC_GFX_LLR_PIPE_GFX})

file(GLOB SRC_GFX_LLR_PIPE_MESH "MeshPipelineVk.*" "IMeshPipeline.*")
source_group("Graphics (low level)/Pipeline/Mesh" FILES ${SRC_GFX_LLR_PIPE_MESH})

file(GLOB SRC_GFX_LLR_PIPE_RT "IRaytracingPipeline.*" "RaytracingPipelineVk.*")
source_group("Graphics (low level)/Pipeline/Rays" FILES ${SRC_GFX_LLR_PIPE_RT})

file(GLOB SRC_GFX_LLR_PIPE_LOUT "IPipelineLayout.*" "PipelineLayoutVk.*")
source_group("Graphics (low level)/Pipeline/Layout" FILES ${SRC_GFX_LLR_PIPE_LOUT})

file(GLOB SRC_GFX_LLR_PIPE "Viewport.*" "PipelineCreateInfo.*" "IPipelineVk.*")
source_group("Graphics (low level)/Pipeline" FILES ${SRC_GFX_LLR_PIPE})

file(GLOB SRC_GFX_LLR_PIPE_SB "ShaderStages.*" "ShaderBindingType.*" "ShaderBindingTypeVk.*")
source_group("Graphics (low level)/Pipeline/Shader bindings" FILES ${SRC_GFX_LLR_PIPE_SB})

file(GLOB SRC_GFX_GEOM "GpuModel3D.*" "GpuMesh3D.*" "GpuGeometryDesc.*" "MeshMapping.*" "PerMeshData.*" "PerModelData.*")
source_group("Graphics (high level)/Geometry" FILES ${SRC_GFX_GEOM})

file(GLOB SRC_GFX_MAT "Material.*" "MaterialInstance.*" "MaterialLayoutPushConstant.*" "MaterialSystem.*" "MaterialType.*")
source_group("Graphics (high level)/Material" FILES ${SRC_GFX_MAT})

file(GLOB SRC_GFX_MAT_LOUT "MaterialLayout.*" "MaterialLayoutBuilder.*" "MaterialLayoutBinding.*" "MaterialLayoutSlot.*")
source_group("Graphics (high level)/Material/Layout" FILES ${SRC_GFX_MAT_LOUT})

file(GLOB SRC_GFX_MAT_LOAD "LoadedMaterialInfo.*" "MaterialLoaderV1.*")
source_group("Graphics (high level)/Material/Loaders" FILES ${SRC_GFX_MAT_LOAD})

file(GLOB SRC_IO "FileIO.*" "Logger.*" "AndroidLogger.*" "PrintableKeys.*")
source_group("IO" FILES ${SRC_IO})

file(GLOB SRC_IO_INPUT_STATE "GamepadState.*" "KeyboardState.*" "MouseModes.*" "MouseState.*" "TouchInput.*")
source_group("IO/Input (states)" FILES ${SRC_IO_INPUT_STATE})

file(GLOB SRC_IO_INPUT "IGamepadInput.*" "IKeyboardInput.*" "IMouseInput.*" "ITouchInput.*" "IUserInput.*")
source_group("IO/Input (base)" FILES ${SRC_IO_INPUT})

file(GLOB SRC_IO_INPUT "InputManager.*" "IInputListener.*")
source_group("IO/Input (listeners)" FILES ${SRC_IO_INPUT})

file(GLOB SRC_IO_INPUT_IN "PcUserInput.*" "AndroidInput.*")
source_group("IO/Input (built-in)" FILES ${SRC_IO_INPUT_IN})

file(GLOB SRC_IO_DISPL "IDisplay.*" "IFullscreenableDisplay.*")
source_group("IO/Display (base)" FILES ${SRC_IO_DISPL})

file(GLOB SRC_IO_DISPL_IN "AndroidDisplay.*" "Window.*")
source_group("IO/Display (built-in)" FILES ${SRC_IO_DISPL_IN})

file(GLOB SRC_EDITOR "Editor.*")
source_group("Editor" FILES ${SRC_EDITOR})

file(GLOB SRC_EDITOR_UI "EditorUi.*" "EditorUiConstants.*" "EditorPropertiesPanel.*" "EditorPanelTitle.*" "EditorObjectList.*" "EditorObjectPropertiesPanel.*" "EditorSystemList.*" "EditorSystemPropertiesPanel.*" "EditorHeader.*" "EditorBottomBar.*")
source_group("Editor/UI" FILES ${SRC_EDITOR_UI})

file(GLOB SRC_EDITOR_CONS "ConsoleCommand.*" "ConsoleCommandExecutor.*" "EditorCommands.*" "ExitCommand.*" "GpuStatsCommand.*")
source_group("Editor/Console" FILES ${SRC_EDITOR_CONS})

file(GLOB SRC_EDITOR_CONT "ColliderRenderSystemController.*" "DeferredRenderSystemController.*" "DeferredRenderSytemController.*" "IComponentController.*" "ModelComponentController3D.*" "PhysicsComponentController3D.*" "PhysicsSystemController.*" "RenderBoundsRenderSystemController.*" "SkyboxRenderSystemController.*" "ISystemController.*")
source_group("Editor/Controllers" FILES ${SRC_EDITOR_CONT})

file(GLOB SRC_EDITOR_VIEWS "ColliderRenderSystemView.*" "DeferredRenderSystemView.*" "IComponentView.*" "ModelComponentView3D.*" "PhysicsComponentView.*" "PhysicsSystemView.*" "RenderBoundsRenderSystemView.*" "SkyboxRenderSystemView.*" "ISystemView.*")
source_group("Editor/Views" FILES ${SRC_EDITOR_VIEWS})

file(GLOB SRC_PERS "Serializer.*" "SchemaId.*" "BinaryBlock.*")
source_group("Persistencia" FILES ${SRC_PERS})

file(GLOB SRC_PHY "IConstraint.*" "DistanceConstraint.*")
source_group("Physics" FILES ${SRC_PHY})

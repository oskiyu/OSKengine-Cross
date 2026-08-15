import os			# Para paths
import sys			# Para argumentos de consola
import shutil		# Para copia de archivos
import subprocess	# Para comandos de consola

def generate(preset : str, android_api : str, game_name : str):
	android_sdk		= os.environ['ANDROID_SDK']
	android_ndk		= os.environ['ANDROID_NDK']
	lib_game 		= f"lib{game_name}.so"

	# Creamos la estructura de carpetas del apk
	apk_structure = '../apk/lib/arm64-v8a/'
	if not os.path.exists(apk_structure):
		os.makedirs(apk_structure)
	if not os.path.exists('../tmp/'):
		os.makedirs('../tmp/')

	# Eliminamos apks antiguos
	if os.path.exists(f'../tmp/{game_name}_tmp.apk'):
		os.remove(f'../tmp/{game_name}_tmp.apk')
	if os.path.exists(f'../tmp/{game_name}_unsigned.apk'):
		os.remove(f'../tmp/{game_name}_unsigned.apk')
	if os.path.exists(f'../tmp/{game_name}.apk'):
		os.remove(f'../tmp/{game_name}.apk')

	# Eliminamos assets antiguos
	if os.path.exists('../tmp/AndroidAssets/'):
		shutil.rmtree("../tmp/AndroidAssets/")
	os.makedirs('../tmp/AndroidAssets/')

	# Copiamos los recursos del juego.
	shutil.copytree('../src/Resources/', '../tmp/AndroidAssets/Resources/')
	shutil.copy2('../src/engine_config.json', '../tmp/AndroidAssets/')

	# Copia de librerias del juego
	if os.path.exists(f"{apk_structure}/{lib_game}"):
		os.remove(f"{apk_structure}/{lib_game}")
	shutil.copy2(f"../bin/{preset}/demo/{lib_game}", apk_structure)

	# Copia de la librería de C++
	if os.path.exists(f"{apk_structure}/libc++_shared.so"):
		os.remove(f"{apk_structure}/libc++_shared.so")
	shutil.copy2(f"{android_ndk}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so", apk_structure)


	# Empaquetamiento del apk
	subprocess.run(f"aapt package -f -M ../src/AndroidManifest.xml -A ../tmp/AndroidAssets/ -F ../tmp/{game_name}_tmp.apk -I {android_sdk}/platforms/android-{android_api}/android.jar -S ../src/AndroidAssets/ ../apk")
	subprocess.run(f"zipalign -f 4 ../tmp/{game_name}_tmp.apk ../tmp//{game_name}_unsigned.apk")

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Uso correcto: {preset} {android_api}")
	else:
		preset 			= sys.argv[1]
		android_api		= sys.argv[2]

		generate(preset, android_api, 'OSKengine_demo')

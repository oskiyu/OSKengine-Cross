import os			# Para paths
import sys			# Para argumentos de consola
import shutil		# Para copia de archivos
import subprocess	# Para comandos de consola

def generate(lib_oskengine : str, manifest_file : str, resources_path : str, android_sdk : str, android_api : str, android_assets : str, apk_name : str):
	# Creamos la estructura de carpetas del apk
	apk_structure = '../apk/lib/arm64-v8a/'
	if not os.path.exists(apk_structure):
		os.makedirs(apk_structure)
	if not os.path.exists('../tmp/'):
		os.makedirs('../tmp/')
		
	if os.path.exists(f'../tmp/{apk_name}.apk'):
		os.remove(f'../tmp/{apk_name}.apk')
		os.remove(f'../tmp/{apk_name}_tmp.apk')

	if os.path.exists('../tmp/AndroidAssets/'):
		shutil.rmtree("../tmp/AndroidAssets/")
	os.makedirs('../tmp/AndroidAssets/')
	shutil.copytree(resources_path , '../tmp/AndroidAssets/Resources/')
	shutil.copy2('../src/engine_config.json', '../tmp/AndroidAssets/')

	# Copia de librerias de OSKengine
	if os.path.exists(f"{apk_structure}/libOSKengine.so"):
		os.remove(f"{apk_structure}/libOSKengine.so")
	shutil.copy2(lib_oskengine , apk_structure)

	# Empaquetamiento del apk
	subprocess.run(f"aapt package -f -M {manifest_file} -A ../tmp/AndroidAssets/ -F ../tmp/{apk_name}_tmp.apk -I {android_sdk}/platforms/android-{android_api}/android.jar -S {android_assets} ../apk")
	subprocess.run(f"zipalign -f 4 ../tmp/{apk_name}_tmp.apk ../tmp/{apk_name}_unsigned.apk")

if __name__ == "__main__":
	lib_oskengine 	= sys.argv[1]
	apk_name	 	= sys.argv[2]
	manifest_file 	= sys.argv[3]
	resources_path	= sys.argv[4]
	android_sdk		= sys.argv[5]
	android_api		= sys.argv[6]
	android_assets	= sys.argv[7]

	generate(lib_oskengine, manifest_file, resources_path, android_sdk, android_api, android_assets, apk_name)

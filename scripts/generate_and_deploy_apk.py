import generate_apk
import deploy_apk

import sys			# Para argumentos de consola

if __name__ == "__main__":
	lib_oskengine 	= sys.argv[1]
	apk_name	 	= sys.argv[2]
	manifest_file 	= sys.argv[3]
	resources_path	= sys.argv[4]
	android_sdk		= sys.argv[5]
	android_api		= sys.argv[6]
	android_assets	= sys.argv[7]

	generate_apk.generate(lib_oskengine, manifest_file, resources_path, android_sdk, android_api, android_assets, apk_name)
	deploy_apk.deploy(f"../tmp/{apk_name}_unsigned.apk")

import subprocess	# Para comandos de consola
import sys			# Para argumentos de consola

def deploy(apk : str):
	subprocess.run(f"adb install -r {apk}")
	
if __name__ == "__main__":
	deploy(sys.argv[1])

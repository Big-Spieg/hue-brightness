main:
	sudo chmod a+rw /dev/ttyACM0
	arduino-cli compile --fqbn arduino:avr:uno -p /dev/ttyACM0 .
	arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno .
	arduino-cli monitor -p /dev/ttyACM0

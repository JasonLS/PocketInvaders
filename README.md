# PocketInvaders

Make sure your PocketBeagle/Beaglebone is up to date if not run, 
"sudo apt-get update"
in terminal.

The LED Matrix i used has 4 wires, Red for power, Black for Ground, BLue for Data Line "DI", Green for Clock Line "CI"

If you are using this with techlab as i did, wiring will be easy, if not, you will be using 
CS pin on the techlab header or P1_8 for CI(Green wire)
MOSI pin on the techlab header or P1_12 for DI(Blue wire)

The Pocketbeagle cannot provide enough amprege for the LED to fully display across all the lines, so to fix this, hook up a 5V 2A wall outlet
to the back of the LED Matrix. The Matrix has 4 wires on each side. One for input and one for output. and has an easily accessable power and ground
wire on the back which will be used for the wall outlet. 


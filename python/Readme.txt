import Dxl

dxl = Dxl.Dynamixel(1)

Dxl.Dynamixel.init("COM6", 2.0)

dxl.write(Dxl.LED, 1)
data = dxl.read(Dxl.Present_Position)

dxl.disable()

Dxl.Dynamixel.close()

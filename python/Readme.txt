from Dxl import *

dxl = Dxl(1)

Dxl.init("COM6", 2.0)

dxl.write(LED, 1)
dxl.read(Present_Position)

dxl.disable()

Dxl.close()

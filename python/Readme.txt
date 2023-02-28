# python 3.11.2
# dynamixel - xc430, xm430, xm540
# package : pyserial
# dynamixel sdk 폴더를 python main 파일로 가져와서 사용함

import Dxl

dxl = Dxl.Dynamixel(1)

Dxl.Dynamixel.init("COM6", 2.0)

dxl.write(Dxl.LED, 1)
data = dxl.read(Dxl.Present_Position)

dxl.disable()

Dxl.Dynamixel.close()

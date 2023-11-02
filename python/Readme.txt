# python 3.11.2
# dynamixel - xc430, xm430, xm540
# package : pyserial
# dynamixel sdk 폴더를 python main 파일로 가져와서 사용함

import warnings # Dxl에 포함돼있으니까 import 안하고 Dxl.warnings.~~~로 해도됨
import Dxl

warnings.filterwarnings("error")

Dxl.Dynamixel.initPort("COM6", 2.0, 115200)

# id 1번 다이나믹셀 선언
dxl = Dxl.Dynamixel(1)

dxl.loadData()

# use
dxl.write(Dxl.LED, 1)
data = dxl.read(Dxl.Present_Position)

dxl.disable()

Dxl.Dynamixel.closePort()

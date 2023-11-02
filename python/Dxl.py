import warnings

import dynamixel_sdk as dynamixel

#EEPROM
#   이름                주소
Model_Number            = 0
Model_Information	    = 2
Firmware_Version	    = 6
ID		    	        = 7
Baud_Rate		        = 8
Return_Delay_Time	    = 9
Drive_Mode		        = 10
Operating_Mode		    = 11
Secondary_ID		    = 12
Protocol_Type		    = 13
Homing_Offset		    = 20
Moving_Threshold	    = 24
Temperature_Limit	    = 31
Max_Voltage_Limit	    = 32
Min_Voltage_Limit	    = 34
PWM_Limit		        = 36
Current_Limit		    = 38#xm430, xm540
Velocity_Limit		    = 44
Max_Position_Limit	    = 48
Min_Position_Limit	    = 52
External_Port_Mode_1    = 56#xm540
External_Port_Mode_2    = 57#xm540
External_Port_Mode_3    = 58#xm540
Startup_Configuration   = 60
Shutdown		        = 63

#RAM
Torque_Enable		    = 64
LED			            = 65
Status_Return_Level	    = 68
Registered_Instruction	= 69
Hardware_Error_Status	= 70
Velocity_I_Gain		    = 76
Velocity_P_Gain		    = 78
Position_D_Gain		    = 80
Position_I_Gain		    = 82
Position_P_Gain		    = 84
Feedforward_2nd_Gain	= 88
Feedforward_1st_Gain	= 90
Bus_Watchdog		    = 98
Goal_PWM		        = 100
Goal_Current		    = 102
Goal_Velocity		    = 104
Profile_Acceleration    = 108
Profile_Velocity	    = 112
Goal_Position		    = 116
Realtime_Tick		    = 120
Moving			        = 122
Moving_Status		    = 123
Present_PWM		        = 124
Present_Current		    = 126#xm430, xm540
Present_Load		    = 126#xc430
Present_Velocity	    = 128
Present_Position	    = 132
Velocity_Trajectory	    = 136
Position_Trajectory	    = 140
Present_Input_Voltage   = 144
Present_Temperature     = 146
Backup_Ready            = 147
External_Port_Data_1    = 152#xm540
External_Port_Data_2    = 154#xm540
External_Port_Data_3    = 156#xm540

RESOLUTION              = 0.087890625

size1Byte = (Firmware_Version, ID, Baud_Rate, Return_Delay_Time, Drive_Mode, Operating_Mode,
            Secondary_ID, Protocol_Type, Temperature_Limit, Shutdown, External_Port_Mode_1, 
            External_Port_Mode_2, External_Port_Mode_3, Startup_Configuration, Torque_Enable, LED, 
            Status_Return_Level, Registered_Instruction, Hardware_Error_Status, Bus_Watchdog,
            Moving, Moving_Status, Present_Temperature, Backup_Ready)
size2Byte = (Model_Number, Max_Voltage_Limit, Min_Voltage_Limit, PWM_Limit, Current_Limit, Velocity_I_Gain,
            Velocity_P_Gain, Position_D_Gain, Position_I_Gain, Position_P_Gain, 
            Feedforward_2nd_Gain, Feedforward_1st_Gain, Goal_PWM, Goal_Current, Realtime_Tick, Present_PWM, 
            Present_Load, Present_Input_Voltage, External_Port_Data_1, External_Port_Data_2, External_Port_Data_3)
size4Byte = (Model_Information, Homing_Offset, Moving_Threshold, Velocity_Limit, Max_Position_Limit, 
            Min_Position_Limit, Goal_Velocity, Profile_Acceleration, Profile_Velocity, 
            Goal_Position, Present_Velocity, Present_Position, Velocity_Trajectory, Position_Trajectory)

class Dynamixel:
    __porthandler = None
    __packethandler = None
    
    def __init__(self, ID):
        self.__id = ID
        self.__max_pos_limit = 4095
        self.__min_pos_limit = 0
        
    @classmethod
    def initPort(cls, port, packet, baudrate = 1000000):
        Dynamixel.__porthandler = dynamixel.PortHandler(port)
        Dynamixel.__packethandler = dynamixel.PacketHandler(packet)

        if Dynamixel.__porthandler.openPort():
            name = Dynamixel.__porthandler.getPortName()
            baudrate = Dynamixel.__porthandler.getBaudRate()
            print(f"Port name - {name}\tbaudrate - {baudrate}")
        else:
            print("Failed open port. Please check the U2D2")

        if Dynamixel.__porthandler.setBaudRate(baudrate):
            baudrate = Dynamixel.__porthandler.getBaudRate()
            print(f"Changed port baudrate - {baudrate}")
        else:
            print("Failed change baudrate. Please check the U2D2")
            
    @classmethod
    def closePort(cls):
        Dynamixel.__porthandler.closePort()
        
    def write(self, address, data):
        size = self.__getByteSize(address)
        result = -1
        error = 0

        if size == 1:
            result, error = Dynamixel.__packethandler.write1ByteTxRx(Dynamixel.__porthandler, self.__id, address, data)
        elif size == 2:
            result, error = Dynamixel.__packethandler.write2ByteTxRx(Dynamixel.__porthandler, self.__id, address, data)
        elif size == 4:
            result, error = Dynamixel.__packethandler.write4ByteTxRx(Dynamixel.__porthandler, self.__id, address, data)
        else:
            warntxt = f"ID: {self.__id}   address Byte size: {size}"
            warnings.warn(warntxt)

        if result != dynamixel.COMM_SUCCESS:
            res = Dynamixel.__packethandler.getTxRxResult(result)
            warntxt = f"ID: {self.__id}    result: {res}    address: {address}"
            warnings.warn(warntxt)
        elif error != 0:
            res = Dynamixel.__packethandler.getRxPacketError(error)
            warntxt = f"ID: {self.__id}    error: {res}    address: {address}"
            warnings.warn(warntxt)
            
    def read(self, address):
        size = self.__getByteSize(address)
        result = -1
        error = 0
        rxdata = 0

        if size == 1:
            rxdata, result, error = Dynamixel.__packethandler.read1ByteTxRx(Dynamixel.__porthandler, self.__id, address)
        elif size == 2:
            rxdata, result, error = Dynamixel.__packethandler.read2ByteTxRx(Dynamixel.__porthandler, self.__id, address)
        elif size == 4:
            rxdata, result, error = Dynamixel.__packethandler.read4ByteTxRx(Dynamixel.__porthandler, self.__id, address)
        else:
            warntxt = f"ID: {self.__id}   address Byte size: {size}"
            warnings.warn(warntxt)

        if result != dynamixel.COMM_SUCCESS:
            res = Dynamixel.__packethandler.getTxRxResult(result)
            warntxt = f"ID: {self.__id}    result: {res}    address: {address}"
            warnings.warn(warntxt)
        elif error != 0:
            res = Dynamixel.__packethandler.getRxPacketError(error)
            warntxt = f"ID: {self.__id}    error: {res}    address: {address}"
            warnings.warn(warntxt)
            
        return rxdata
    
    def disable(self):
        self.write(LED, 0)
        self.write(Torque_Enable, 0)
        
    def checkMove(self, pos):
        if pos > self.__max_pos_limit or pos < self.__min_pos_limit:
            return False
        return True
    
    def loadData(self):
        self.__max_pos_limit = self.read(Max_Position_Limit)
        self.__min_pos_limit = self.read(Min_Position_Limit)
    
    def __getByteSize(self, address):
        size = 0
        if address in size1Byte:
            size = 1
        elif address in size2Byte:
            size = 2
        elif address in size4Byte:
            size = 4
        else:
            print(f"Exception : address - {address}")
        return size
    

# 2048(position) == 0(degree)
def angle2pos(angle):
    return int(round((angle+180)/RESOLUTION))
def pos2angle(pos):
    return pos * RESOLUTION - 180

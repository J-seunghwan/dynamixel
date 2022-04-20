#pragma once
#include <iostream>
#include <cassert>

#include "port_handler_windows.h"
#include "packet_handler.h"

//EEPROM		ÀÌ¸§		 ÁÖ¼Ò	Å©±â
#define Model_Number			0	//2
#define Model_Information		2	//4
#define Firmware_Version		6	//1
#define ID						7	//1
#define	Baud_Rate				8	//1
#define	Return_Delay_Time		9	//1
#define Drive_Mode				10	//1
#define Operating_Mode			11	//1
#define Secondary_ID			12	//1
#define Protocol_Type			13	//1
#define Homing_Offset			20	//4
#define Moving_Threshold		24	//4
#define Temperature_Limit		31	//1
#define	Max_Voltage_Limit		32	//2
#define Min_Voltage_Limit		34	//2
#define PWM_Limit				36	//2
#define Current_Limit			38	//2 xm430
#define Velocity_Limit			44	//4
#define Max_Position_Limit		48	//4
#define	Min_Position_Limit		52	//4
#define	Shutdown				63	//1
//RAM
#define Torque_Enable			64	//1
#define LED						65	//1
#define Status_Return_Level		68	//1
#define Registered_Instruction	69	//1
#define Hardwre_Error_Status	70	//1
#define Velocity_I_Gain			76	//2
#define Velocity_P_Gain			78	//2
#define Position_D_Gain			80	//2
#define Position_I_Gain			82	//2
#define Position_P_Gain			84	//2
#define Feedforward_2nd_Gain	88	//2
#define Feedforward_1st_Gain	90	//2
#define Bus_Watchdog			98	//1
#define Goal_PWM				100	//2
#define Goal_Current			102	//2
#define Goal_Velocity			104	//4
#define Profile_Acceleration	108	//4
#define Profile_Velocity		112	//4
#define Goal_Position			116	//4
#define Realtime_Tick			120	//4
#define Moving					122	//1
#define Moving_Status			123 //1
#define Present_PWM				124	//2
#define Present_Current			126 //2 xm430
#define Present_Load			126	//2 xc430
#define Present_Velocity		128 //4
#define Present_Position		132	//4
#define Velocity_Trajectory		136	//4
#define Position_Trajectory		140	//4
#define Present_Input_Voltage	144 //2
#define Present_Temperature		146	//1

#define RESOLUTION 0.087890625

class Dxl
{
public:
	Dxl(int id);
	~Dxl();

	static void init(int baudrate = 1000000);
	static void close();
	void write(int address, int data, dynamixel::PacketHandler* packet);
	int read(int address, dynamixel::PacketHandler* packet);
	void disable(dynamixel::PacketHandler* packet);
	bool checkMove(int pos);

	int max_pos_limit = 4095;
	int min_pos_limit = 0;

private:
	int getByteSize(int address);

	int _id = -1;
	static dynamixel::PortHandler* port;
	//static dynamixel::PacketHandler* packet;
};

int angle2pos(double angle);
double pos2angle(int pos);

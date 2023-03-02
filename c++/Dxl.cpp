#include "Dxl.h"

Dxl::Dxl(int id)
{
	this->_id = id;
}

Dxl::~Dxl()
{
	
}

//static
void Dxl::initPort(int baudrate)
{
	bool open = handler.port->openPort();
	if (open) {
		std::cout << "port name - " << handler.port->getPortName() << "   port baud rate - " << handler.port->getBaudRate() << std::endl;
	}
	else{
		std::cout << "Failed open port. Please check the port" << std::endl;
	}
	
	bool change = handler.port->setBaudRate(baudrate);
	if (change) {
		std::cout << "Chagned port baud rate - " << handler.port->getBaudRate() << std::endl;
	}
	else {
		std::cout << "Failed change baudrate. Please check the U2D2" << std::endl;
	}
}

//static
void Dxl::closePort()
{
	handler.port->closePort();
}

void Dxl::write(int address, int data)
{
	int size = getByteSize(address);
	int result = -1;
	uint8_t error = 0;

	switch (size)
	{
	case 1:
		result = this->handler.packet->write1ByteTxRx(this->handler.port, this->_id, address, data, &error);
		break;
	case 2:
		result = this->handler.packet->write2ByteTxRx(this->handler.port, this->_id, address, data, &error);
		break;
	case 4:
		result = this->handler.packet->write4ByteTxRx(this->handler.port, this->_id, address, data, &error);
		break;
	default:
		std::cout << "ID - " << this->_id << " Byte size - " << size << std::endl;
		break;
	}

	if (result != COMM_SUCCESS) {
		std::cout << "ID - " << this->_id << " " << this->handler.packet->getTxRxResult(result) << std::endl;
	}
	else if (error != 0) {
		std::cout << "ID - " << this->_id << " " << this->handler.packet->getRxPacketError(error) << std::endl;
	}
}

int Dxl::read(int address)
{
	int size = getByteSize(address);
	int result = -1;
	uint8_t error = 0;
	int rxdata = 0;

	switch (size)
	{
	case 1:
		result = this->handler.packet->read1ByteTxRx(this->handler.port, this->_id, address, (uint8_t*)&rxdata, &error);
		break;
	case 2:
		result = this->handler.packet->read2ByteTxRx(this->handler.port, this->_id, address, (uint16_t*)&rxdata, &error);
		break;
	case 4:
		result = this->handler.packet->read4ByteTxRx(this->handler.port, this->_id, address, (uint32_t*)&rxdata, &error);
		break;
	default:
		std::cout << "read error ID - " << this->_id << " Byte size - " << size << std::endl;
		break;
	}

	if (result != COMM_SUCCESS) {
		std::cout << "ID - " << this->_id << " " << this->handler.packet->getTxRxResult(result) << std::endl;
	}
	else if (error != 0) {
		std::cout << "ID - " << this->_id << " " << this->handler.packet->getRxPacketError(error) << std::endl;
	}
	
	return rxdata;
}

void Dxl::disable() {
	Dxl::write(LED, 0);
	Dxl::write(Torque_Enable, 0);
}

bool Dxl::checkMove(int pos)
{
	if (pos > this->max_pos_limit || pos < this->min_pos_limit) {
		return false;
	}
	return true;
}

void Dxl::loadData()
{
	this->max_pos_limit = this->read(Max_Position_Limit);
	this->min_pos_limit = this->read(Min_Position_Limit);
}

int Dxl::getByteSize(int address)
{
	int size = 0;

	switch (address)
	{
	case Firmware_Version:
	case ID:
	case Baud_Rate:
	case Return_Delay_Time:
	case Drive_Mode:
	case Operating_Mode:
	case Secondary_ID:
	case Protocol_Type:
	case Temperature_Limit:
	case Shutdown:
	case Torque_Enable:
	case LED:
	case Status_Return_Level:
	case Registered_Instruction:
	case Hardwre_Error_Status:
	case Bus_Watchdog:
	case Moving:
	case Moving_Status:
	case Present_Temperature:
	case External_Port_Mode_1:
	case External_Port_Mode_2:
	case External_Port_Mode_3:
	case Startup_Configuration:
	case Backup_Ready:
		size = 1;
		break;

	case Model_Number:
	case Max_Voltage_Limit:
	case Min_Voltage_Limit:
	case PWM_Limit:
	case Velocity_I_Gain:
	case Velocity_P_Gain:
	case Position_D_Gain:
	case Position_I_Gain:
	case Position_P_Gain:
	case Feedforward_2nd_Gain:
	case Feedforward_1st_Gain:
	case Goal_PWM:
	case Realtime_Tick:
	case Present_PWM:
	case Present_Load:
	case Present_Input_Voltage:
	case External_Port_Data_1:
	case External_Port_Data_2:
	case External_Port_Data_3:
		size = 2;
		break;

	case Model_Information:
	case Homing_Offset:
	case Moving_Threshold:
	case Velocity_Limit:
	case Max_Position_Limit:
	case Min_Position_Limit:
	case Goal_Velocity:
	case Profile_Acceleration:
	case Profile_Velocity:
	case Goal_Position:
	case Present_Velocity:
	case Present_Position:
	case Velocity_Trajectory:
	case Position_Trajectory:
		size = 4;
		break;

	default:
		std::cout << "other size " << size << std::endl;
		break;
	}

	return size;
}

int angle2pos(double angle)
{
	return round((angle + 180) / RESOLUTION);
}

double pos2angle(int pos)
{
	return pos * RESOLUTION - 180;
}

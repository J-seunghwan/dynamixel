#include "Dxl.h"

Dxl::Dxl(int id)
{
	this->_id = id;
}

Dxl::~Dxl()
{
	
}

void Dxl::init(int baudrate)
{
	bool open = port->openPort();
	if (open) {
		std::cout << "Port name - " << port->getPortName() << "    baud rate - " << port->getBaudRate() << std::endl;
	}
	else{
		std::cout << "Failed open port. Please check the port" << std::endl;
	}
	assert(open != false);


	bool change = port->setBaudRate(baudrate);
	if (change) {
		std::cout << "Chagned baud rate - " << port->getBaudRate() << std::endl;
	}
	else {
		std::cout << "Failed change baudrate. Please check the U2D2" << std::endl;
	}
	assert(change != false);
}

void Dxl::close()
{
	port->closePort();
}

void Dxl::write(int address, int data, dynamixel::PacketHandler* packet)
{
	int size = getByteSize(address);
	int result = -1;
	uint8_t error = 0;

	switch (size)
	{
	case 1:
		result = packet->write1ByteTxRx(port, this->_id, address, data, &error);
		break;
	case 2:
		result = packet->write2ByteTxRx(port, this->_id, address, data, &error);
		break;
	case 4:
		result = packet->write4ByteTxRx(port, this->_id, address, data, &error);
		break;
	default:
		std::cout << "ID - " << this->_id << " Byte size - " << size << std::endl;
		break;
	}

	if (result != COMM_SUCCESS) {
		std::cout << "ID - " << this->_id << " " << packet->getTxRxResult(result) << std::endl;
	}
	else if (error != 0) {
		std::cout << "ID - " << this->_id << " " << packet->getRxPacketError(error) << std::endl;
	}
}

int Dxl::read(int address, dynamixel::PacketHandler* packet)
{
	int size = getByteSize(address);
	int result = -1;
	uint8_t error = 0;
	int rxdata = 0;

	switch (size)
	{
	case 1:
		result = packet->read1ByteTxRx(port, this->_id, address, (uint8_t*)&rxdata, &error);
		break;
	case 2:
		result = packet->read2ByteTxRx(port, this->_id, address, (uint16_t*)&rxdata, &error);
		break;
	case 4:
		result = packet->read4ByteTxRx(port, this->_id, address, (uint32_t*)&rxdata, &error);
		break;
	default:
		std::cout << "read error ID - " << this->_id << " Byte size - " << size << std::endl;
		break;
	}

	if (result != COMM_SUCCESS) {
		std::cout << "ID - " << this->_id << " " << packet->getTxRxResult(result) << std::endl;
	}
	else if (error != 0) {
		std::cout << "ID - " << this->_id << " " << packet->getRxPacketError(error) << std::endl;
	}
	
	return rxdata;
}

void Dxl::disable(dynamixel::PacketHandler* packet) {
	Dxl::write(LED, 0, packet);
	Dxl::write(Torque_Enable, 0, packet);
}

bool Dxl::checkMove(int pos)
{
	if (pos > this->max_pos_limit || pos < this->min_pos_limit) {
		return false;
	}
	return true;
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

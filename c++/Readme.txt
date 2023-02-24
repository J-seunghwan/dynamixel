/*
환경 및 지원 모델
os - windows10
다이나믹셀 - xc430, xm430, xm540
*/

#include "Dxl.h"

// port, protocol 지정
Handler Dxl::handler = {
	dynamixel::PortHandler::getPortHandler("COM3"),
	dynamixel::PacketHandler::getPacketHandler(2.0)
};

int main(){
	// id 번호의 다이나믹셀 선언
	Dxl dxl = Dxl(1); // 1번 id 다이나믹셀

	// 다이나믹셀 연결 시도
	Dxl::init();

	// 사용 예시
	dxl.write(Torque_Enable, 1);
	dxl.read(Present_Position);
	
	// 연결 끊기 전 사용(다이나믹셀 비활성화) - 기본적으로 led, torque 비활성
	dxl.disable(packet);
	
	// 다이나믹셀과 연결 끊기
	Dxl::close();

	return 0;
}

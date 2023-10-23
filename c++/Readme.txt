/*
환경 및 지원 모델
os - Windows10
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

	// U2D2 초기화
	Dxl::initPort();
	
	// 다이나믹셀에 저장된 값 불러오기
	dxl.loadData();

	// 사용 예시
	dxl.write(Torque_Enable, 1);
	dxl.read(Present_Position);
	
	// 연결 끊기 전 led, torque 비활성
	dxl.disable();
	
	// U2D2 연결 끊기
	Dxl::closePort();

	return 0;
}

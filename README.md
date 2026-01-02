# MAX6921 VFD Arduino Driver Board


## 개요
아두이노를 사용하여 VFD(Vacuum Fluorescent Display)를 구동하는 범용 PCB 회로 제작 프로젝트입니다.

## 목표
- max6921 드라이버 칩을 사용한 VFD 구동 회로 설계 
- 아두이노와 호환되는 범용 PCB 보드 제작 
- 향후 다른 VFD 드라이버 칩 지원 확장 예정

## 주요 특징
- max6921 VFD 드라이버 칩 기반
- 아두이노 호환 인터페이스
- 범용적인 VFD 구동 솔루션
- **다중 MAX6921 칩 지원 (공통 제어 핀 사용)**
- **파워 드라이버 보드 별도 제거로 범용성 강화**

## 파워 드라이버

VFD 구동을 위한 전원 회로 설계에 대한 자세한 내용은 [power_driver.md](power_driver.md)를 참조하세요.

## VFD 예제

<details>
<summary>1번 VFD (7BT317NK)</summary>

![Grid/Segment](pics/VFD/7bt317b.jpg)
![VFD](pics/VFD/7bt317a.jpg)

연결 테이블
![connection table](pics/connection_table_7BT317NK.jpg)

</details>

## 라이선스
MIT License

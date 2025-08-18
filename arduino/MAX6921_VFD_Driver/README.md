# MAX6921 VFD 드라이버 라이브러리

MAX6921 드라이버 IC를 사용하여 VFD(진공형광디스플레이) 모듈을 제어하는 아두이노 라이브러리입니다.

## 주요 기능

- VFD 제어를 위한 사용하기 쉬운 인터페이스
- 문자 및 숫자 표시 지원
- 밝기 제어
- 멀티플렉스 디스플레이를 위한 그리드 스캔
- 포괄적인 테스트 기능
- 최적화된 SPI 통신

## 지원 하드웨어

- **VFD 디스플레이**: 7BT317NK (7개 그리드, 21개 세그먼트를 가진 4자리 VFD)
- **드라이버 IC**: MAX6921 (20채널 VFD 드라이버)
- **마이크로컨트롤러**: Arduino Uno/Nano/Pro Mini 또는 호환 보드

## 하드웨어 연결

### MAX6921AWI 핀 구성
MAX6921AWI IC의 제어 핀들:
- **DIN (Data Input)**: 2번 핀 - 시리얼 데이터 입력
- **CLK (Clock)**: 15번 핀 - 시리얼 클록 
- **LOAD (Latch)**: 16번 핀 - 데이터 래치 신호
- **BLANK (Output Enable)**: 13번 핀 - 출력 활성화/비활성화

### 다중 MAX6921 구성 (권장)
여러 개의 MAX6921 칩을 사용할 때 모든 제어 신호를 공유합니다:

| 기능         | 아두이노 핀  | MAX6921AWI 핀 | 설명            |
|-------------|-------------|--------------|----------------|
| SPI 데이터   | D11 (MOSI)  | 2번 핀 (DIN) | 공유 데이터 입력  |
| SPI 클록     | D13 (SCK)   | 15번 핀 (CLK) | 공유 클록        |
| 래치        | D10 (SS)    | 16번 핀 (LOAD)| 공통 래치 신호   |
| 디스플레이 활성화 | D9      | 13번 핀 (BLANK)| 공통 디스플레이 활성화 |

**연결 방법**: MAX6921 칩들은 SPI를 통해 데이지 체인으로 연결됩니다. 첫 번째 칩의 DOUT이 두 번째 칩의 DIN에 연결되는 방식입니다.

## 설치 방법

1. 이 라이브러리를 다운로드하거나 클론합니다
2. **중요**: `VFD_7BT317NK_Font` 라이브러리도 함께 설치해야 합니다 (필수 의존성)
3. 두 라이브러리 폴더를 아두이노 라이브러리 디렉토리에 복사합니다:
   - `MAX6921_VFD_Driver`
   - `VFD_7BT317NK_Font`
4. 아두이노 IDE를 재시작합니다
5. 라이브러리를 포함합니다: `#include <MAX6921_VFD_Driver.h>`

## 의존성

이 라이브러리는 `VFD_7BT317NK_Font` 라이브러리가 필요합니다. 폰트 라이브러리는 7BT317NK VFD 디스플레이의 폰트 패턴을 포함하며 다음을 제공합니다:

- 44개 문자 패턴 (0-9, A-Z, 특수 문자)
- 최적화된 21비트 세그먼트 패턴
- 문자 검색 함수
- 디버그 유틸리티

폰트 패턴은 `vfd-configs/7bt317nk/` 디렉토리에 정의된 세그먼트 매핑을 기반으로 합니다.

## 기본 사용법

```cpp
#include <MAX6921_VFD_Driver.h>

// 기본 핀으로 드라이버 인스턴스 생성 (LOAD=D10, BLANK=D9)
MAX6921_VFD_Driver vfd;

// 또는 사용자 정의 핀 지정
// MAX6921_VFD_Driver vfd(12, 8);  // loadPin, blankPin

void setup() {
  // 드라이버 초기화
  if (!vfd.begin()) {
    Serial.println("VFD 초기화 실패!");
    return;
  }
  
  // 텍스트 표시
  vfd.displayString("HELLO");
  
  // 밝기 설정 (0-255)
  vfd.setBrightness(128);
}

void loop() {
  // 디스플레이 업데이트 (적절한 그리드 스캔을 위해 정기적으로 호출)
  vfd.update();
}
```

void setup() {
  // Initialize the driver
  vfd.begin();
  
  // Set brightness (0-255)
  vfd.setBrightness(200);
  
  // Display text
  vfd.displayString("HELLO");
}

void loop() {
  // Must call refresh() regularly to update display
  vfd.refresh();
  
  // Your code here...
}
```

## API 참조

### 초기화
- `bool begin()` - 기본 SPI 속도로 초기화
- `bool begin(uint32_t spiClockSpeed)` - 사용자 정의 SPI 속도로 초기화

### 디스플레이 제어
- `void clear()` - 디스플레이 지우기
- `void refresh()` - 디스플레이 업데이트 (루프에서 정기적으로 호출)
- `void setBrightness(uint8_t brightness)` - 밝기 설정 (0-255)
- `uint8_t getBrightness()` - 현재 밝기 얻기

### 텍스트 표시
- `void displayString(const char* text)` - 문자열 표시 (최대 4글자)
- `void displayString(String text)` - 아두이노 String 표시
- `void displayCharacter(uint8_t position, char character)` - 단일 문자 표시

### 숫자 표시
- `void displayNumber(int number)` - 정수 표시
- `void displayNumber(long number)` - 긴 정수 표시
- `void displayFloat(float number, uint8_t decimals)` - 소수 표시

### 저수준 제어
- `void setSegment(uint8_t grid, uint8_t segment, bool state)` - 개별 세그먼트 제어
- `void setGrid(uint8_t grid, uint32_t segmentMask)` - 그리드의 모든 세그먼트 설정

### 테스트 함수
- `void displayTest()` - 기본 디스플레이 테스트
- `void segmentTest()` - 모든 세그먼트 테스트
- `void gridTest()` - 모든 그리드 테스트

## 예제

라이브러리에는 여러 예제 스케치가 포함되어 있습니다:

1. **SimpleDisplay** - 기본 사용법 예제
2. **DisplayTest** - 시리얼 명령어가 포함된 포괄적인 테스트 스위트

## 핀 사용자 정의

생성자에서 제어 핀을 사용자 정의할 수 있습니다:

```cpp
// 사용자 정의 핀 할당
MAX6921_VFD_Driver vfd(
  10, // MAX6921 #1 LOAD 핀
  9,  // MAX6921 #1 BLANK 핀
  8,  // MAX6921 #2 LOAD 핀
  7   // MAX6921 #2 BLANK 핀
);
```

## 주의사항

- 메인 루프에서 항상 `refresh()`를 정기적으로 호출해야 합니다
- 라이브러리는 하드웨어 SPI를 사용합니다 (Uno에서 11번, 13번 핀)
- VFD는 고전압 전원 공급이 필요합니다 (별도 구매)
- 7BT317NK VFD 디스플레이로 테스트되었습니다

## 라이선스

이 라이브러리는 MIT 라이선스 하에 배포됩니다.

## 기여

기여를 환영합니다! GitHub에서 풀 리퀘스트를 제출하거나 이슈를 보고해 주세요.

## 제작자

Your Name

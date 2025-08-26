# VFD 드라이버 폰트 분리 및 공통 핀 사용 수정 사항

## 수정 개요
1. 7BT317NK VFD의 연결 및 폰트 테이블을 참조하여 별도의 폰트 파일을 만들고, 메인 VFD 드라이버에서 해당 폰트 파일을 참조하도록 수정
2. **다중 MAX6921 칩에서 LOAD와 BLANK 핀을 공통으로 사용하도록 변경**

## 생성된 파일

### 1. VFD_7BT317NK_Font 라이브러리
**위치**: `arduino/VFD_7BT317NK_Font/`

#### VFD_7BT317NK_Font.h
- 44개 문자의 폰트 패턴 선언 (0-9, A-Z, 특수문자)
- 21비트 세그먼트 패턴 (P0-P20)
- 함수 선언: `getVFDCharacterPattern()`, `getVFDFontTableSize()` 등

#### VFD_7BT317NK_Font.cpp
- 실제 폰트 패턴 데이터 구현
- vfd-configs/7bt317nk의 폰트 테이블을 기반으로 한 정확한 비트 패턴
- 문자 검색 및 유틸리티 함수 구현

#### keywords.txt
- 문법 강조를 위한 키워드 정의

#### library.properties
- 라이브러리 메타데이터 정의

#### README.md
- 폰트 라이브러리 사용법 및 세그먼트 매핑 설명

## 수정된 파일

### 1. MAX6921_VFD_Driver.h
**수정 사항:**
- `#include "../VFD_7BT317NK_Font/VFD_7BT317NK_Font.h"` 추가
- 기존 `FontPattern` 구조체 정의 제거
- 기존 `VFD_FONT_TABLE` 선언 제거
- **핀 정의 변경**: 개별 칩별 핀 → 공통 핀 사용
  ```cpp
  // 이전
  #define DEFAULT_MAX6921_1_LOAD_PIN    10
  #define DEFAULT_MAX6921_1_BLANK_PIN   9
  #define DEFAULT_MAX6921_2_LOAD_PIN    8
  #define DEFAULT_MAX6921_2_BLANK_PIN   7
  
  // 현재
  #define DEFAULT_LOAD_PIN    10   // 공통 LOAD 핀
  #define DEFAULT_BLANK_PIN   9    // 공통 BLANK 핀
  ```
- **생성자 매개변수 변경**: 4개 핀 → 2개 핀
  ```cpp
  // 이전
  MAX6921_VFD_Driver(uint8_t load1Pin, uint8_t blank1Pin, 
                     uint8_t load2Pin, uint8_t blank2Pin);
  
  // 현재
  MAX6921_VFD_Driver(uint8_t loadPin = DEFAULT_LOAD_PIN,
                     uint8_t blankPin = DEFAULT_BLANK_PIN);
  ```

### 2. MAX6921_VFD_Driver.cpp
**수정 사항:**
- 기존 로컬 폰트 테이블 제거
- `getCharacterPattern()` 메서드를 외부 폰트 라이브러리 함수 호출로 변경:
  ```cpp
  uint32_t MAX6921_VFD_Driver::getCharacterPattern(char character) {
      return getVFDCharacterPattern(character);
  }
  ```
- **생성자 매개변수 변경**: 4개 핀 → 2개 핀
- **핀 초기화 함수 간소화**: `initializePins()` 함수에서 4개 핀 설정 → 2개 핀 설정
- **데이터 전송 함수 최적화**: `sendData()` 함수에서 개별 LOAD 핀 제어 → 공통 LOAD 핀 제어
  ```cpp
  // 이전
  digitalWrite(_load1Pin, LOW);
  digitalWrite(_load2Pin, LOW);
  // ... SPI 전송 ...
  digitalWrite(_load1Pin, HIGH);
  digitalWrite(_load2Pin, HIGH);
  
  // 현재
  digitalWrite(_loadPin, LOW);
  // ... SPI 전송 ...
  digitalWrite(_loadPin, HIGH);
  ```

### 3. MAX6921_VFD_Driver/README.md
**수정 사항:**
- 의존성 섹션 추가
- VFD_7BT317NK_Font 라이브러리 설치 안내 추가
- 폰트 라이브러리의 기능 설명 추가

## 새로 생성된 예제

### 1. FontTest.ino
**위치**: `arduino/MAX6921_VFD_Driver/examples/FontTest/`
- 모든 문자의 폰트 패턴을 테스트하는 종합적인 예제
- 숫자, 문자, 특수문자를 순차적으로 테스트
- 시리얼 출력으로 테스트 진행상황 확인

### 2. FontIntegrationTest.ino
**위치**: `arduino/FontIntegrationTest/`
- 폰트 라이브러리 통합이 제대로 작동하는지 확인하는 간단한 테스트
- 모든 문자의 비트 패턴을 시리얼로 출력
- 컴파일 및 링크 오류 확인용

## 폰트 데이터 세부사항

### 세그먼트 매핑
- P0-P20: 21개 세그먼트
- 각 문자는 21비트 패턴으로 표현 (P0=LSB, P20=MSB)
- MSB First 전송 방식: bit20 → bit19 → ... → bit1 → bit0
- vfd-configs/7bt317nk/connection-table.md의 매핑을 정확히 반영

### 지원 문자
- **숫자**: 0-9 (10개)
- **문자**: A-Z (26개)  
- **특수문자**: +, -, *, /, :, _, ., 공백 (8개)
- **총 44개 문자**

## 사용법

### 라이브러리 설치
1. `MAX6921_VFD_Driver` 폴더를 Arduino 라이브러리 디렉토리에 복사
2. `VFD_7BT317NK_Font` 폴더를 Arduino 라이브러리 디렉토리에 복사
3. Arduino IDE 재시작

### 코드에서 사용
```cpp
#include <MAX6921_VFD_Driver.h>  // 자동으로 폰트 라이브러리도 포함됨

MAX6921_VFD_Driver vfd(10, 9, 8, 7);  // LOAD1, BLANK1, LOAD2, BLANK2

void setup() {
    vfd.begin();
    vfd.displayString("ABCD");  // 폰트 라이브러리에서 자동으로 패턴 검색
}
```

## 주요 장점

1. **모듈성**: 폰트 데이터가 별도 라이브러리로 분리되어 유지보수 용이
2. **재사용성**: 다른 VFD 프로젝트에서도 폰트 라이브러리 재사용 가능
3. **정확성**: vfd-configs 폴더의 정확한 매핑 데이터 기반
4. **확장성**: 새로운 문자 추가 시 폰트 라이브러리만 수정하면 됨
5. **디버깅**: 폰트 패턴 확인을 위한 유틸리티 함수 제공

## 테스트 방법

1. `FontIntegrationTest.ino`를 업로드하여 시리얼 모니터에서 폰트 패턴 확인
2. `FontTest.ino`를 업로드하여 실제 VFD에서 모든 문자 표시 테스트
3. 기존 `SimpleDisplay.ino` 및 `DisplayTest.ino` 예제로 정상 동작 확인

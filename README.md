# HKNU IoT Team Project
> Hybus-Smart4412 타겟시스템을 이용한 메모리게임


## 개발 기간
> 2024.05 ~ 2024.06


## 팀 구성
|팀원|소속|깃허브|
|:---:|:---:|:---:|
|송규림|한경대 컴퓨터공학과 SC|[@gyuliming](https://github.com/gyuliming)|
|유지원|한경대 컴퓨터공학과 SC|[@syy669](https://github.com/syy669)|
|정아현|한경대 컴퓨터공학과 SC|[@greenblueredgreen](https://github.com/greenblueredgreen)|
|조정행|한경대 컴퓨터공학과 SC|[@JOJeongHaeng](https://github.com/JOJeongHaeng)|


## 기술 스택 및 환경
- C
- Linux
- Hybus-Smart4412


## 게임 규칙
플레이어가 무작위로 생성되는 패턴을 기억한 후, 정확히 재현하는 게임이다.

게임의 목표는 각 레벨과 모드에 따라 점점 더 복잡해지는 패턴을 정확히 기억하고 맞춰서 목표 점수를 획득하는 것이다.

1. 플레이어가 `모드: 4x4, 6x6, 8x8`와 `난이도: Easy, Normal, Hard`를 선택한다.
2. 선택한 모드와 난이도에 따라 `Dot Matrix`에 `무작위 패턴`이 생성된다.
3. 무작위로 생성되는 패턴은 플레이어가 선택한 `모드`와 `난이도`에 따라 크기와 개수가 달라진다.
4. 패턴이 `5초` 동안 점등되었다가 소등되며, 플레이어는 `Tact Switch`를 사용해 이를 재현해야 한다.
5. 패턴을 정확히 재현하면, 플레이어는 `20점`을 획득하고 누적 점수가 출력된다.
6. 누적 점수 `100점`을 달성하면 `승리 문구`가 출력되고 게임이 종료된다.
7. 만약 플레이어가 패턴 재현에 실패하면, `패턴 일치 실패 문구`가 출력되며 게임이 종료된다.


## 플로우 차트
<p align="left"><img src="/img/flowchart.png"></img></p>


## 게임 플레이 영상


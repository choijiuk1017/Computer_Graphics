# TANK BLITZ

> DirectX 11 렌더링 파이프라인과 HLSL Shader를 직접 구성한 3D 그래픽스 프로젝트

DirectX 11을 기반으로 Device와 Swap Chain 초기화부터 모델, 텍스처, 조명, Skybox, Skeletal Animation 및 충돌 판정까지 직접 구현한 개인 프로젝트입니다.

OBJ 및 FBX 모델을 로드하고 Ambient, Diffuse, Specular 조명과 다중 Point Light를 적용했으며 Normal Map 기반 Bump Mapping, Multi Texture Blending, Cube Mapping, AABB 충돌 및 발사체 시스템을 구현했습니다.

---

## 플레이 영상

[![TANK BLITZ 플레이 영상](https://img.youtube.com/vi/qI17PK0DxRk/0.jpg)](https://www.youtube.com/watch?v=qI17PK0DxRk)

- YouTube: https://www.youtube.com/watch?v=qI17PK0DxRk

---

## 프로젝트 개요

| 항목 | 내용 |
| --- | --- |
| 프로젝트명 | TANK BLITZ |
| 프로젝트 형태 | 개인 프로젝트 |
| 개발 기간 | 2025.03 ~ 2025.12 |
| 그래픽 API | DirectX 11 |
| 개발 언어 | C++, HLSL |
| 개발 환경 | Visual Studio 2022 |
| Platform Toolset | v143 |
| 플랫폼 | Windows |
| 빌드 대상 | Win32, x64 |
| 빌드 구성 | Debug, Release |

---

## 기술 스택

### Graphics

- DirectX 11
- Direct3D 11
- HLSL
- Vertex Shader
- Pixel Shader
- Constant Buffer
- Depth Stencil
- Rasterizer State
- Alpha Blending

### Resource

- OBJ
- FBX
- DDS Texture
- Assimp
- Skeletal Animation
- Cube Map
- Normal Map

### Gameplay

- Keyboard and Mouse Input
- Camera
- Projectile
- AABB Collision
- Sound
- Bitmap UI
- Text Rendering

---

# 핵심 구현 내용

- Direct3D Device, Device Context 및 Swap Chain을 초기화했습니다.
- Back Buffer, Render Target View와 Depth Stencil Buffer를 구성했습니다.
- World, View, Projection 행렬 기반 렌더링 파이프라인을 구현했습니다.
- OBJ 및 FBX 모델을 로드했습니다.
- DDS Texture와 Texture Array를 적용했습니다.
- Ambient, Diffuse, Specular 조명 연산을 구현했습니다.
- 최대 4개의 Light를 사용하는 다중 Point Light를 구현했습니다.
- Normal Map 기반 Bump Mapping을 적용했습니다.
- 여러 Texture를 혼합하는 Multi Texture Mapping을 구현했습니다.
- Cube Map 기반 Skybox를 구현했습니다.
- FBX 캐릭터와 Skeletal Animation을 구현했습니다.
- 카메라 이동과 마우스 시점 조작을 구현했습니다.
- 조준 상태와 발사체 시스템을 구현했습니다.
- 월드 좌표로 변환한 AABB 기반 충돌 판정을 구현했습니다.
- FPS, CPU 사용량, 탄약 및 상태 정보를 화면에 출력했습니다.
- Bitmap 기반 타이틀 및 조준 UI를 구현했습니다.
- 사운드 로딩과 재생 기능을 구현했습니다.

---

# 렌더링 파이프라인

```text
Windows Application 시작
    ↓
SystemClass 초기화
    ↓
Direct3D Device 및 Swap Chain 생성
    ↓
Render Target 생성
    ↓
Depth Stencil Buffer 생성
    ↓
Camera 및 Projection Matrix 설정
    ↓
Model / Texture / Shader 로드
    ↓
Frame Update
    ↓
World / View / Projection Matrix 갱신
    ↓
Vertex Shader
    ↓
Rasterizer
    ↓
Pixel Shader
    ↓
Back Buffer 출력
    ↓
Swap Chain Present
```

---

# 주요 시스템

## 1. SystemClass

`SystemClass`는 Windows 애플리케이션의 실행과 종료를 관리합니다.

### 주요 역할

- Window 생성
- Windows Message Loop
- 입력 처리
- 프레임 갱신
- Graphics 시스템 호출
- 애플리케이션 종료
- 리소스 해제

```text
main.cpp
    ↓
SystemClass::Initialize()
    ↓
SystemClass::Run()
    ↓
Message Loop
    ↓
SystemClass::Frame()
    ↓
GraphicsClass::Frame()
    ↓
GraphicsClass::Render()
```

---

## 2. D3DClass

`D3DClass`는 Direct3D 11의 핵심 객체와 렌더 상태를 관리합니다.

### 주요 역할

- Direct3D Device 생성
- Device Context 생성
- Swap Chain 생성
- Back Buffer 생성
- Render Target View 생성
- Depth Stencil Buffer 생성
- Depth Stencil State 생성
- Rasterizer State 생성
- Viewport 설정
- Projection Matrix 생성
- World Matrix 생성
- Orthographic Matrix 생성
- Alpha Blending 상태 관리
- Z-Buffer 상태 관리

### 기본 렌더링 설정

```cpp
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
```

---

## 3. GraphicsClass

`GraphicsClass`는 Scene에 필요한 모델, 카메라, 조명, Shader, UI, 사운드와 게임 로직을 통합 관리합니다.

### 주요 함수

```cpp
bool Initialize(int ScreenWidth, int ScreenHeight, HWND Hwnd);

void Shutdown();

bool Frame(int MouseX, int MouseY);

bool Render();

void SetAmbient(int Value);
void SetDiffuse(int Value);
void SetSpecular(int Value);

void IncreaseIntensity();
void DecreaseIntensity();

void SetAniNum(int AnimationNumber);

void ShootBullet();

void Aiming();
void UnAiming();
```

### 관리 대상

- Direct3D
- Camera
- Static Model
- Multi Texture Model
- FBX Model
- Skin Model
- Animation
- Light
- Skybox
- Shader
- Bullet
- Text
- Bitmap
- Sound

---

## 4. 프레임 처리

```text
입력 상태 확인
    ↓
Delta Time 계산
    ↓
Player 위치와 회전 갱신
    ↓
Camera 위치와 방향 갱신
    ↓
Animation Time 갱신
    ↓
발사체 이동
    ↓
AABB 충돌 검사
    ↓
Scene Render
    ↓
Text 및 UI Render
    ↓
Back Buffer Present
```

게임 로직과 렌더링에 필요한 시간을 Timer를 통해 계산하고 프레임 속도에 따라 이동과 Animation이 달라지지 않도록 구성했습니다.

---

## 5. 카메라 시스템

`CameraClass`와 `Player`에서 카메라 위치, 회전과 바라보는 방향을 관리합니다.

### Player 입력

```cpp
void Update(float Duration);

void MouseInput();
void KeyboardInput(float Duration);
```

### 주요 데이터

```cpp
float _speed;
float _mouseSpeed;

XMFLOAT3 _position;
XMFLOAT3 _rotation;

XMVECTOR _lookAtVec;
```

키보드 입력으로 위치를 변경하고 마우스 입력으로 회전 값을 갱신합니다.

Player의 위치와 시선 방향을 Camera에 반영해 3D 공간을 이동할 수 있도록 구현했습니다.

---

## 6. 모델 로딩

프로젝트에서는 여러 모델 클래스를 목적에 따라 구분했습니다.

| 클래스 | 역할 |
| --- | --- |
| `ModelClass` | 기본 정적 모델 |
| `BaseModel` | 모델 공통 데이터 |
| `ModelLoader` | 외부 모델 파일 로딩 |
| `FBXModelClass` | FBX 모델 처리 |
| `SkinModel` | Skeletal Mesh 렌더링 |
| `RMModel` | Resource Manager 기반 모델 |
| `MultiTextureModelClass` | 여러 Texture를 사용하는 모델 |

### 모델 처리 흐름

```text
모델 파일 로드
    ↓
Vertex와 Index 데이터 생성
    ↓
Vertex Buffer 생성
    ↓
Index Buffer 생성
    ↓
Texture Resource 연결
    ↓
World Matrix 설정
    ↓
Shader에 Buffer 전달
    ↓
DrawIndexed()
```

---

## 7. FBX 및 Skeletal Animation

FBX 캐릭터 모델과 Animation 데이터를 로드해 Skeletal Animation을 재생합니다.

### 관련 클래스

- `FBXModelClass`
- `ModelLoader`
- `SkinModel`
- `Animation`
- `AlignedAllocationPolicy`

### 처리 방식

1. FBX 또는 Assimp를 통해 모델 데이터를 읽습니다.
2. Mesh의 Vertex, Index와 Bone 정보를 구성합니다.
3. 각 Vertex의 Bone Index와 Weight를 저장합니다.
4. Animation 시간에 따라 Bone Transform을 계산합니다.
5. Parent Bone의 Transform을 결합합니다.
6. 최종 Bone Matrix를 Constant Buffer에 전달합니다.
7. `skinShader.hlsl`의 Vertex Shader에서 Skinning을 적용합니다.

---

## 8. 조명 시스템

Ambient, Diffuse, Specular 조명 요소를 분리해 제어할 수 있도록 구성했습니다.

### 조명 요소

- Ambient Light
- Diffuse Light
- Specular Light
- Light Direction
- Light Position
- Specular Power
- Light Intensity

### 조명 제어 함수

```cpp
void SetAmbient(int Value);
void SetDiffuse(int Value);
void SetSpecular(int Value);

void IncreaseIntensity();
void DecreaseIntensity();
```

---

## 9. 다중 Point Light

`GraphicsClass`는 최대 4개의 `LightClass` 객체를 관리합니다.

```cpp
LightClass* m_Light1;
LightClass* m_Light2;
LightClass* m_Light3;
LightClass* m_Light4;
```

각 Light의 위치와 색상 데이터를 HLSL Constant Buffer에 전달하고 Pixel Shader에서 Pixel 위치와 광원 위치 사이의 거리 및 방향을 계산합니다.

### Point Light 계산 흐름

```text
Pixel World Position
    ↓
Light Position과 방향 계산
    ↓
거리 계산
    ↓
Normal과 Light Vector의 내적
    ↓
Diffuse 값 계산
    ↓
거리 감쇠 적용
    ↓
각 광원의 결과 합산
    ↓
최종 Pixel Color 출력
```

---

## 10. Bump Mapping

Normal Map을 사용해 실제 Mesh의 Polygon을 추가하지 않고 표면의 미세한 굴곡을 표현했습니다.

### 관련 파일

- `BumpMapShaderClass.cpp`
- `BumpMapShaderClass.h`
- `bumpmapShader.hlsl`

### 처리 흐름

```text
Diffuse Texture Sample
    ↓
Normal Map Sample
    ↓
Normal 값을 -1 ~ 1 범위로 변환
    ↓
Tangent / Binormal / Normal 행렬 적용
    ↓
Pixel 단위 Normal 계산
    ↓
Light Vector와 내적
    ↓
조명 결과 출력
```

---

## 11. Multi Texture Mapping

여러 Texture를 하나의 모델에 적용하고 혼합해 지형과 표면을 표현했습니다.

### 관련 클래스

- `MultiTextureModelClass`
- `TextureArrayClass`
- `TextureShaderClass`
- `Multi Texture HLSL Shader`

### 처리 방식

1. 여러 Texture를 Shader Resource View 배열로 전달합니다.
2. Blend Map 또는 Alpha 값을 읽습니다.
3. Texture별 가중치를 계산합니다.
4. 여러 Texture Color를 혼합합니다.
5. 최종 표면 색상을 출력합니다.

---

## 12. Cube Mapping과 Skybox

Cube Map Texture를 사용해 카메라 주변의 배경 공간을 표현했습니다.

### 관련 클래스 및 Shader

- `SkyboxClass`
- `skymapshaders.hlsl`
- `skymap.dds`

### 처리 흐름

```text
Cube Mesh 생성
    ↓
Cube Map Texture 연결
    ↓
Skybox를 Camera 위치로 이동
    ↓
Vertex 방향을 Texture 좌표로 사용
    ↓
Cube Texture Sampling
    ↓
배경 출력
```

Skybox를 카메라 위치에 계속 배치해 플레이어가 이동해도 배경과의 실제 거리가 변하지 않는 것처럼 표현했습니다.

Skybox는 Cube Map Texture를 사용하므로 이 프로젝트에서 구현한 방식은 Cube Mapping에 해당합니다.

---

## 13. HLSL Shader

| Shader | 역할 |
| --- | --- |
| `lightShader.hlsl` | 기본 조명 계산 |
| `pointLightShader.hlsl` | 다중 Point Light |
| `bumpmapShader.hlsl` | Normal Map 기반 Bump Mapping |
| `maintextureShader.hlsl` | 메인 Texture 렌더링 |
| `textureShader.hlsl` | 일반 Texture 출력 |
| `skinShader.hlsl` | Skeletal Animation Skinning |
| `skymapshaders.hlsl` | Cube Map 기반 Skybox |
| `fontShader.hlsl` | Text UI 렌더링 |

---

## 14. 발사체 시스템

`GraphicsClass`에서 발사체 모델과 이동 상태를 관리합니다.

### 주요 데이터

```cpp
ModelClass* m_Bullet = nullptr;

XMMATRIX m_BulletWorld = XMMatrixIdentity();

XMFLOAT3 m_BulletPos;
XMFLOAT3 m_BulletDir;

bool m_bulletFlying = false;

int bulletNum = 30;

float m_BulletLifeTime = 0.0f;
const float BULLET_MAX_LIFETIME = 2.0f;
```

### 발사체 흐름

```text
발사 입력
    ↓
현재 Camera 위치와 방향 확인
    ↓
발사체 위치 및 이동 방향 설정
    ↓
m_bulletFlying 활성화
    ↓
Delta Time 기반 위치 갱신
    ↓
Bullet World Matrix 생성
    ↓
AABB 충돌 검사
    ↓
충돌 또는 최대 수명 도달
    ↓
발사체 비활성화
```

발사체는 최대 2초 동안 활성 상태로 이동하며 충돌하거나 수명이 종료되면 비활성화됩니다.

---

## 15. AABB 충돌 판정

모델의 로컬 AABB를 World Matrix로 변환해 월드 공간에서 충돌 여부를 검사합니다.

### World AABB 계산

```cpp
void CalculateAABB(
    const std::vector<XMFLOAT3>& LocalBox,
    const XMMATRIX& World,
    XMVECTOR& OutMin,
    XMVECTOR& OutMax
);
```

로컬 Bounding Box의 8개 꼭짓점을 World Matrix로 변환한 뒤 X, Y, Z의 최소값과 최대값을 구합니다.

### 교차 검사

```cpp
bool AABB_Intersect(
    XMVECTOR MinA,
    XMVECTOR MaxA,
    XMVECTOR MinB,
    XMVECTOR MaxB
);
```

### 판정 방식

```text
A의 최대 X < B의 최소 X → 충돌하지 않음
A의 최소 X > B의 최대 X → 충돌하지 않음

A의 최대 Y < B의 최소 Y → 충돌하지 않음
A의 최소 Y > B의 최대 Y → 충돌하지 않음

A의 최대 Z < B의 최소 Z → 충돌하지 않음
A의 최소 Z > B의 최대 Z → 충돌하지 않음

모든 축이 겹침 → 충돌
```

---

## 16. UI 및 상태 출력

### 관련 클래스

- `BitmapClass`
- `TextClass`
- `FontClass`
- `FontShaderClass`
- `FPSClass`
- `CPUClass`

### 출력 정보

- 타이틀 이미지
- 조준경
- FPS
- CPU 사용량
- 탄약
- 게임 상태 Text

3D Scene 렌더링 후 Z-Buffer와 Alpha Blending 상태를 전환해 2D Bitmap과 Text를 출력합니다.

---

## 17. 사운드

`SoundClass`에서 사운드 장치 초기화와 재생을 관리합니다.

게임 시작, 발사 및 기타 게임 이벤트에서 사운드를 재생할 수 있도록 Graphics 시스템과 연결했습니다.

---

# 트러블 슈팅

## 모델마다 서로 다른 로딩 및 렌더링 코드가 중복되는 문제

### 문제

OBJ, FBX 및 Skin Model을 추가하면서 모델마다 Vertex Buffer, Index Buffer, Texture와 Shader를 관리하는 코드가 반복되는 문제가 있었습니다.

### 해결

모델의 공통 구조를 `BaseModel`에 분리하고 모델 로딩을 `ModelLoader`, 렌더링 리소스를 `RMModel`, `RMTexture`, `RMShader` 등으로 구분했습니다.

### 결과

모델 종류에 따라 필요한 처리만 확장하고 공통 리소스 초기화와 해제 코드를 재사용할 수 있게 되었습니다.

### 배운 점

그래픽스 프로그래밍에서는 렌더링 결과뿐만 아니라 GPU Resource의 생성, 소유권과 해제 책임을 명확히 구분하는 것이 중요하다는 점을 배웠습니다.

---

## 발사체가 회전하거나 이동한 모델과 정확히 충돌하지 않는 문제

### 문제

모델의 로컬 좌표 AABB를 그대로 충돌 검사에 사용하면 모델이 이동하거나 회전한 이후 화면에 보이는 위치와 충돌 영역이 일치하지 않는 문제가 발생했습니다.

### 해결

로컬 AABB의 8개 꼭짓점을 현재 모델의 World Matrix로 변환한 뒤 월드 좌표의 최소값과 최대값을 다시 계산했습니다.

변환한 두 월드 AABB가 X, Y, Z 모든 축에서 겹치는지 검사했습니다.

### 결과

모델의 World Transform을 반영한 위치에서 발사체와 오브젝트의 충돌을 검사할 수 있게 되었습니다.

### 배운 점

충돌 영역과 렌더링 모델이 서로 다른 좌표계를 사용하면 오차가 발생하므로 충돌 검사 전에 동일한 좌표계로 변환해야 한다는 점을 배웠습니다.

---

## Skybox가 카메라 이동에 따라 가까워지는 문제

### 문제

Skybox를 일반적인 월드 오브젝트처럼 고정된 위치에 배치하면 카메라가 이동하면서 배경 Mesh의 표면에 가까워지는 문제가 발생했습니다.

### 해결

매 Frame Skybox의 위치를 Camera 위치에 맞추고 Cube Map Texture를 방향 Vector로 Sampling했습니다.

### 결과

카메라가 이동해도 배경이 항상 먼 거리에 있는 것처럼 보이도록 표현할 수 있게 되었습니다.

### 배운 점

Skybox는 일반적인 Scene Object와 달리 위치보다 카메라의 시선 방향을 중심으로 렌더링해야 한다는 점을 배웠습니다.

---

## 고해상도 모델에서 표면 디테일을 높이면 Polygon 수가 증가하는 문제

### 문제

모델의 작은 표면 굴곡을 Geometry만으로 표현하면 Vertex와 Polygon 수가 증가해 렌더링 비용이 커지는 문제가 있었습니다.

### 해결

Tangent Space Normal Map을 이용한 Bump Mapping을 구현해 실제 Geometry를 추가하지 않고 Pixel Shader에서 표면 방향을 변경했습니다.

### 결과

모델의 Polygon 수를 늘리지 않고 조명에 반응하는 표면의 미세한 굴곡을 표현할 수 있게 되었습니다.

### 배운 점

실시간 렌더링에서는 Geometry와 Pixel Shader 중 어느 단계에서 디테일을 표현할지 성능과 품질을 고려해 선택해야 한다는 점을 배웠습니다.

---

# 주요 클래스

| 클래스 | 역할 |
| --- | --- |
| `SystemClass` | Windows Message Loop와 프로그램 수명주기 |
| `D3DClass` | Direct3D Device, Swap Chain 및 Render State |
| `GraphicsClass` | Scene 렌더링과 게임 로직 |
| `CameraClass` | View Matrix와 카메라 상태 |
| `Player` | 입력 기반 이동과 시점 |
| `ModelClass` | 기본 정적 모델 |
| `ModelLoader` | 외부 모델 로딩 |
| `FBXModelClass` | FBX 모델 처리 |
| `SkinModel` | Skeletal Mesh |
| `Animation` | Animation 데이터 |
| `LightClass` | 광원 데이터 |
| `LightShaderClass` | 조명 Shader 제어 |
| `BumpMapShaderClass` | Bump Mapping |
| `MultiTextureModelClass` | Multi Texture 모델 |
| `TextureArrayClass` | Texture 배열 관리 |
| `SkyboxClass` | Cube Map Skybox |
| `SoundClass` | Sound 재생 |
| `TextClass` | Text UI |
| `BitmapClass` | 2D Bitmap UI |

---

# 프로젝트 구조

```text
Computer_Graphics2_PA03_C177038_CJU/
├─ Framework.sln
│
└─ Framework/
   ├─ main.cpp
   │
   ├─ SystemClass.*
   ├─ D3DClass.*
   ├─ GraphicsClass.*
   │
   ├─ CameraClass.*
   ├─ Player.*
   │
   ├─ BaseModel.*
   ├─ ModelClass.*
   ├─ ModelLoader.*
   ├─ FBXModelClass.*
   ├─ SkinModel.*
   ├─ Animation.*
   │
   ├─ LightClass.*
   ├─ LightShaderClass.*
   ├─ BumpMapShaderClass.*
   ├─ MultiTextureModelClass.*
   ├─ TextureArrayClass.*
   ├─ SkyboxClass.*
   │
   ├─ RMModel.*
   ├─ RMShader.*
   ├─ RMTexture.*
   ├─ ResourceManager.*
   │
   ├─ BitmapClass.*
   ├─ TextClass.*
   ├─ FontClass.*
   ├─ FontShaderClass.*
   ├─ FPSClass.*
   ├─ CPUClass.*
   ├─ SoundClass.*
   ├─ TimeManager.*
   │
   ├─ include/
   ├─ lib/
   │
   └─ data/
      ├─ bumpmapShader.hlsl
      ├─ fontShader.hlsl
      ├─ lightShader.hlsl
      ├─ maintextureShader.hlsl
      ├─ pointLightShader.hlsl
      ├─ skinShader.hlsl
      ├─ skymapshaders.hlsl
      ├─ textureShader.hlsl
      ├─ skymap.dds
      └─ Texture 및 Model Resource
```

---

# 빌드 방법

## 요구 환경

- Windows 10/11
- Visual Studio 2022
- MSVC Platform Toolset v143
- Windows 10 SDK
- DirectX 11 지원 GPU

## 프로젝트 실행

```bash
git clone https://github.com/choijiuk1017/Computer_Graphics.git
```

다음 경로로 이동합니다.

```text
Computer_Graphics/
└─ 과제/
   └─ CG2/
      └─ Computer_Graphics2_PA03_C177038_CJU/
```

1. `Framework.sln`을 Visual Studio 2022로 엽니다.
2. Platform Toolset이 `v143`인지 확인합니다.
3. 우선 `Release / Win32` 구성을 선택합니다.
4. `Framework/include`, `Framework/lib`, `Framework/data`의 상대 경로가 유지되는지 확인합니다.
5. Solution을 빌드합니다.
6. 실행 시 리소스를 찾지 못하면 Working Directory를 `Framework`로 설정합니다.

> 외부 Library와 Shader 및 Texture를 상대 경로로 불러오므로 프로젝트 내부 디렉터리 구조를 변경하면 실행되지 않을 수 있습니다.

---

# 개발 과정에서 얻은 경험

- Direct3D 11 Device와 Swap Chain 초기화
- Render Target과 Depth Stencil 구성
- World, View, Projection Matrix 처리
- Vertex 및 Index Buffer 생성
- HLSL Vertex Shader와 Pixel Shader 작성
- Ambient, Diffuse, Specular 조명 연산
- 다중 Point Light 구현
- Normal Map 기반 Bump Mapping
- Multi Texture Mapping
- Cube Mapping 기반 Skybox
- OBJ 및 FBX 모델 로딩
- Skeletal Animation과 Skinning
- 발사체 이동과 수명 관리
- World Space AABB 충돌 판정
- GPU Resource 생성과 해제 관리
- Bitmap 및 Text UI 렌더링

---

# Repository

https://github.com/choijiuk1017/Computer_Graphics/tree/main/%EA%B3%BC%EC%A0%9C/CG2/Computer_Graphics2_PA03_C177038_CJU

#include <gctypes.h>
#include "main.h"

#define VPAD_BUTTON_A        0x8000
#define VPAD_BUTTON_B        0x4000
#define VPAD_BUTTON_X        0x2000
#define VPAD_BUTTON_Y        0x1000
#define VPAD_BUTTON_LEFT     0x0800
#define VPAD_BUTTON_RIGHT    0x0400
#define VPAD_BUTTON_UP       0x0200
#define VPAD_BUTTON_DOWN     0x0100

typedef struct
{
	f32 x,y;
} Vec2D;

typedef struct
{
    float x,y,z;
} Vec3D;


typedef struct
{
	u16 x,y;
	u16 touched;
	u16 invalid;
} VPADTPData;

typedef struct
{
    Vec3D X,Y,Z;
} VPADOrientation;

typedef struct
{
    uint32_t buttonHold;
    uint32_t buttonTrigger;
    uint32_t buttonRelease;
    Vec2D leftStick, rightStick;
    Vec3D acc;
    f32 accValue;
    f32 accSpeed;
    Vec2D accVertical;
    Vec3D gyro;
    Vec3D angle;
    s8 vpadErr;
    VPADTPData tpdata;
    VPADTPData tpdata1;
    VPADTPData tpdata2;
    VPADOrientation dir;
    BOOL headphoneStatus;
    Vec3D magnet;
    u8 volume;
    u8 batteryLevel;
    u8 micStatus;
    u8 volumeCalibrated;
    u8 __paddings__[7];
} VPADData;
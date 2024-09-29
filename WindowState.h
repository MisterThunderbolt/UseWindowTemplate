#if !defined(WINDOWSTATE_H)

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <float.h>

#define Assert(Expression) if (!(Expression)) { __debugbreak(); }
#define InvalidCodePath Assert(!"Invalid Code Path")

#define WindowName "WindowTamplate"
#define WindowStartWidth 1280
#define WindowStartHeight 720

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef int32_t b32;

#define global static
#define local_global static

struct global_state 
{
	b32 IsRunning;
	HWND WindowHandle;
	HDC DeviceContext;

	u32 FrameBufferWidth;
	u32 FrameBufferHeight;
	u32* FrameBufferPixels;
};

#define WINDOWSTATE_H
#endif
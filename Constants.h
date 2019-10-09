
HDC hdcScreen = GetWindowDC(GetDesktopWindow());

int Width		  = GetDeviceCaps(hdcScreen, HORZRES);
int Height		  = GetDeviceCaps(hdcScreen, VERTRES) - 38;
int DirectXWidth  = Width - 464;
int DirectXHeight = Height - 200;
int DirectXLeft   = 217;
int DirectXTop    = 100;

float PI   = 3.1415f;

#define LEFT_BUTTON 0
#define RIGHT_BUTTON 1
#define MIDDLE_BUTTON 2

#define ROTATION_AXIS_X 1
#define ROTATION_AXIS_Y 2
#define ROTATION_AXIS_Z 3

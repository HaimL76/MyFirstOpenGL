/*      Steven Billington
        January 13, 2003
        May 26, 2003 - UPDATE
        RobotOGL.cpp
        rod@cprogramming.com

        The following program creates a window and then
        uses multiple OpenGL functions to display a
        animated robot constructed of different size
        cubes. To compile this code you must make the
        proper library links in project--->settings.

        I apologize for any amount of scattered code or
        mis-formatting that printing this may occur. Please
        feel free to email me at the address above for the .cpp
        file.
*/

/*      These are what we refer to as Pre-processor
        Directives. In order for certain functions in
        C++ to operate you must include certain header
        files. Each header file below contains different
        functions needed throughout this program.
*/

#pragma comment(linker, "/subsystem:windows")

#include <iostream>
#include <sstream>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "MyTime.h"
#include <time.h>
#include <Windowsx.h>
//#include <gl/glaux.h>
#include "OpenGl.h"

using namespace std;

/*      Here we find a few global variables. While
        i don't really like to use global variables,
        i found them very handy for this particular
        program. These variables will control angles,
        fullscreen, and the global device context.
*/

void MyOpenGL::Draw()
{
    vector<DrawingObject*>::const_iterator itr;
    
    for (itr = vec.begin(); itr != vec.end(); itr++)
        (*itr)->Draw();
}

int width, height;

HDC g_HDC;
float angle = 0.0f;
float legAngle[2] = { 0.0f, 0.0f };
float armAngle[2] = { 0.0f, 0.0f };
bool fullScreen = false;

/*      Function:       DrawCube
        Purpose:        As the name would suggest, this is
                                the function for drawing the cubes.
*/

float baseX = 0.0f;
float baseY = 0.0f;

float length = 10.0f;

int color = 0;

int xMousePos, yMousePos;

void DrawLine(
    //float xPos, float yPos, float zPos, 
    HWND& hWnd)
{
    float red = (color & 0xFF) / 256.0f;
    float green = (color >> 8 & 0xFF) / 256.0f;
    float blue = (color++ >> 16 & 0xFF) / 256.0f;

    std::stringstream stream;
    stream << std::fixed
        //<< std::setprecision(2)
        << red << ", " << green << ", " << blue;
    stream.str(string());
    stream << std::fixed << xMousePos << ", " << yMousePos;

    //SetWindowTextA(hWnd, stream.str().c_str());// LPCSTR lpString);

    SetWindowTextA(hWnd, stream.str().c_str());// LPCSTR lpString);


    //glColor3f(red, green, blue);

    glBegin(GL_LINES);

    float x1 = 0.1f;// = xMousePos / (float)width;// baseX;
    float y1 = 0.1f;// yMousePos / (float)height;// baseY;

    float x2 = x1;// +1.0f;
    float y2 = y1 + 1.0f;

    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    glEnd();

    //baseX += 0.01f;
    //baseY += 0.01f;
}

float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;

const float stepX = 0.1f;
float transX = 0.1f;
int dirX = 1;

const float stepY = 0.1f;
float transY = 0.1f;
int dirY = 1;

//const int angleLimit = 30;

int myAngle = 0;
const float stepAngle = 1.0f;
int dirAngle = 1;

int angleLimit = 10.0f;
int angleLimitHalf = 10.0f / 2.0f;

void MyOpenGL::Render()
{
    /*      Enable depth testing
    */
    glEnable(GL_DEPTH_TEST);

    /*      Heres our rendering. Clears the screen
            to black, clear the color and depth
            buffers, and reset our modelview matrix.
    */
    glClearColor(0, 0, 0, 0);// red += 0.1f, green += 0.1f, blue += 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /*      Increase rotation angle counter
    */
    angle = angle + 1.0f;

    /*      Reset after we have completed a circle
    */
    if (angle >= 360.0f)
    {
        angle = 0.0f;
    }

    std::stringstream stream;
    stream << std::fixed 
        //<< std::setprecision(2)
        << transX;

    //SetWindowTextA(hWnd, stream.str().c_str());// LPCSTR lpString);

    glPushMatrix();
    glLoadIdentity();

    /*      Move to 0,0,-30 , rotate the robot on
            its y axis, draw the robot, and dispose
            of the current matrix.
    */
    transX += dirX * stepX;
    transY += dirY * stepY;

    int iSecret, iGuess;

    float ang = rand() % angleLimit - angleLimitHalf;

    myAngle++;// += ang;

    float angle = myAngle * 3.14 / 180;

    transX += stepX * cos(angle) * dirX;//(float)(dirX * stepX) * cos(myAngle);
    transY += stepY * sin(angle) * dirY;//(float)(dirY * stepY) * sin(myAngle);

    //transX = 10.0f * xMousePos / (float) height;
    //transY = 10.0f * yMousePos / (float) width * -1;

    length += 0.01f * dirX;

    if (length > 11 || length < 9)
        dirX * -1;

    transX = xMousePos / (float)width + length * cos(angle);
    transY = yMousePos / (float)height + length * sin(angle);

    glTranslatef(transX, transY, -30.0f);
    glRotatef(myAngle * -1, transX, transY, -30.0f);

    if (abs(transX) > 20)
        dirX *= -1;

    if (abs(transY) > 20)
        dirY *= -1;

    //glTranslatef(0.0f, 0.0f, 0.0f);
    //glRotatef(angle, 0.0f, 1.0f, 0.0f);
    //DrawRobot(0.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

    Draw();

    glPopMatrix();

    glFlush();

    /*      Bring back buffer to foreground
    */
    SwapBuffers(g_HDC);
}

void MyOpenGL::Add(DrawingObject* obj)
{
    vec.push_back(obj);
}

//function to set the pixel format for the device context
/*      Function:       SetupPixelFormat
        Purpose:        This function will be responsible
                                for setting the pixel format for the
                                device context.
*/
void SetupPixelFormat(HDC hDC)
{
    /*      Pixel format index
    */
    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
            1,                                      //default version
            PFD_DRAW_TO_WINDOW |                    //window drawing support
            PFD_SUPPORT_OPENGL |                    //opengl support
            PFD_DOUBLEBUFFER,                       //double buffering support
            PFD_TYPE_RGBA,                          //RGBA color mode
            32,                                     //32 bit color mode
            0, 0, 0, 0, 0, 0,                       //ignore color bits
            0,                                      //no alpha buffer
            0,                                      //ignore shift bit
            0,                                      //no accumulation buffer
            0, 0, 0, 0,                             //ignore accumulation bits
            16,                                     //16 bit z-buffer size
            0,                                      //no stencil buffer
            0,                                      //no aux buffer
            PFD_MAIN_PLANE,                         //main drawing plane
            0,                                      //reserved
            0, 0, 0 };                              //layer masks ignored

            /*      Choose best matching format*/
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);

    /*      Set the pixel format to the device context*/
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}


/*      Windows Event Procedure Handler
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*      Rendering and Device Context
            variables are declared here.
    */
    static HGLRC hRC;
    static HDC hDC;

    /*      Width and Height for the
            window our robot is to be
            displayed in.
    */


    switch (message)
    {
    case WM_MOUSEMOVE:
        xMousePos = GET_X_LPARAM(lParam);
        yMousePos = GET_Y_LPARAM(lParam);
        break;

    case WM_CREATE: //window being created

        hDC = GetDC(hwnd);  //get current windows device context
        g_HDC = hDC;
        SetupPixelFormat(hDC); //call our pixel format setup function

        /*      Create rendering context and make it current
        */
        hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hRC);

        return 0;
        break;

    case WM_CLOSE:  //window is closing

            /*      Deselect rendering context and delete it*/
        wglMakeCurrent(hDC, NULL);
        wglDeleteContext(hRC);

        /*      Send quit message to queue*/
        PostQuitMessage(0);

        return 0;
        break;

    case WM_SIZE:

        /*      Retrieve width and height*/
        height = HIWORD(lParam);
        width = LOWORD(lParam);

        /*      Don't want a divide by 0*/
        if (height == 0)
        {
            height = 1;
        }

        /*      Reset the viewport to new dimensions*/
        glViewport(0, 0, width, height);

        /*      Set current Matrix to projection*/
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); //reset projection matrix

        /*      Time to calculate aspect ratio of
                our window.
        */
        gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

        glMatrixMode(GL_MODELVIEW); //set modelview matrix
        glLoadIdentity(); //reset modelview matrix

        return 0;
        break;

    default:

        break;
    }

    return (DefWindowProc(hwnd, message, wParam, lParam));
}

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    WNDCLASSEX windowClass; //window class
    HWND    hwnd;                   //window handle
    MSG             msg;                    //message
    bool    done;                   //flag for completion of app
    DWORD   dwExStyle;              //window extended style
    DWORD   dwStyle;                //window style
    RECT    windowRect;

    /*      Screen/display attributes*/
    int width = 800;
    int height = 600;
    int bits = 32;

    windowRect.left = (long)0;               //set left value to 0
    windowRect.right = (long)width;  //set right value to requested width
    windowRect.top = (long)0;                //set top value to 0
    windowRect.bottom = (long)height;//set bottom value to requested height

    /*      Fill out the window class structure*/
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = L"MyClass";
    windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    /*      Register window class*/
    if (!RegisterClassEx(&windowClass))
    {
        return 0;
    }

    /*      Check if fullscreen is on*/
    if (fullScreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;   //screen width
        dmScreenSettings.dmPelsHeight = height; //screen height
        dmScreenSettings.dmBitsPerPel = bits;   //bits per pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN !=
            DISP_CHANGE_SUCCESSFUL))
        {
            /*      Setting display mode failed, switch to windowed*/
            MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
            fullScreen = false;
        }
    }

    /*      Check if fullscreen is still on*/
    if (fullScreen)
    {
        dwExStyle = WS_EX_APPWINDOW;    //window extended style
        dwStyle = WS_POPUP;                             //windows style
        ShowCursor(FALSE);                              //hide mouse pointer
    }

    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; //window extended style
        dwStyle = WS_OVERLAPPEDWINDOW;                                  //windows style
    }

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    /*      Class registerd, so now create our window*/
    hwnd = CreateWindowEx(NULL, L"MyClass",  //class name
        L"OpenGL Robot",       //app name
        dwStyle |
        WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS,
        0, 0,                         //x and y coords
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,//width, height
        NULL,                 //handle to parent
        NULL,                 //handle to menu
        hInstance,    //application instance
        NULL);                //no xtra params

/*      Check if window creation failed (hwnd = null ?)*/
    if (!hwnd)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);      //display window
    UpdateWindow(hwnd);                     //update window

    done = false;   //initialize loop condition variable

    SYSTEMTIME sysTime1, sysTime2;

    GetSystemTime(&sysTime1);

    /* initialize random seed: */
    srand(time(NULL));

    MyOpenGL myOpenGL;

    myOpenGL.Add(new LineObject);

    /*      Main message loop*/
    while (!done)
    {
        PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

        if (msg.message == WM_QUIT)     //did we receive a quit message?
        {
            done = true;
        }

        else
        {
            GetSystemTime(&sysTime2);

            long long timediff = Subtract(sysTime2, sysTime1);

            if (timediff > 12650 * 4.5)
            {
                sysTime1 = sysTime2;

                myOpenGL.Render();
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (fullScreen)
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(TRUE);
    }

    return msg.wParam;

}
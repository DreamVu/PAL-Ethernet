# include <GL/glut.h>
# include <stdio.h>
# include "PAL.h"


GLuint CreateTexture(int width, int height, int bpp, unsigned char* data)
{
    GLuint handle;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    GLenum format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    return handle;    
}

static bool g_bUserRGB = true;
unsigned char* ConvertBGR2RGB(int width, int height, unsigned char *src, unsigned char* temp)
{
    if(!g_bUserRGB) return src;
    
    unsigned char* dst = temp;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            
            src += 3;
            dst += 3;
        }
    }
    return temp;
}

static float g_fDisparityScale = 0.5f;
void ConvertDisparity2RGB(int width, int height, unsigned short *src, unsigned char* dst)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float value = g_fDisparityScale*(*src++);
            if(value > 255.0f) value = 255.0f;
            else if(value < 0.0f) value = 0.0f;
            
            *dst++ = (unsigned char)value;
            *dst++ = (unsigned char)value;
            *dst++ = (unsigned char)value;
        }
    }
}



void UpdateTexture(GLuint handle, int width, int height, unsigned char* src, unsigned char* temp)
{
    temp = ConvertBGR2RGB(width, height, src, temp);
    glBindTexture(GL_TEXTURE_2D, handle);
    GLenum format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, temp);   
}


void UpdateDisparity(GLuint handle, int width, int height, unsigned char* src, unsigned char* dst)
{
    ConvertDisparity2RGB(width, height, (unsigned short*)src, dst);
    glBindTexture(GL_TEXTURE_2D, handle);
    GLenum format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dst);   
}




void ActivateTexture(GLuint handle)
{
    glBindTexture(GL_TEXTURE_2D, handle);

	// make the texture repeat in the u and v texture directions
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// linearly filter the texture when it needs to be magnified
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// linearly filter textures into the distance
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void DestroyTexture(GLuint handle)
{
    glDeleteTextures(1, &handle);
}



static GLuint g_texLeft;
static GLuint g_texRight;
static GLuint g_texDisparity;
static unsigned char* g_pTempBuffer = 0;

static int g_glutWindow = -1;

void OnWindowResize(int width, int height)
{
    int left = 0;
    int bottom = 0;

    glViewport(left, bottom, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}



void OnExit()
{
    PAL::Destroy();
    
    DestroyTexture(g_texLeft);
    DestroyTexture(g_texRight);
    DestroyTexture(g_texDisparity);
    
    glutDestroyWindow(g_glutWindow);   
    g_glutWindow = -1;
    
    if(g_pTempBuffer)
    {
        delete[] g_pTempBuffer;
        g_pTempBuffer = 0;
    }
}


void OnMouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state ==  GLUT_DOWN)
    {
        g_bUserRGB = !g_bUserRGB;
    }    
}

void OnKeyPress(unsigned char key, int x, int y)
{

# define ESC_Key 27

    switch (key)
    {
        case ESC_Key:
            OnExit();
        break;
        
        case 'w':
        case 'W':
            g_fDisparityScale *= 1.1f;
        break;
            
        case 's':
        case 'S':
            g_fDisparityScale *= 0.9f;
        break;
            
        case ' ':
            g_fDisparityScale = 1.0f;
        break;
        
    }
}


void DrawStrips(GLuint* textures, int count)
{    
    float strip_height = 2.0f/(float)count;
    float y1 = -1.0f;
    float y2 = y1 + strip_height;
    
    for(int i = 0; i < count; i++)
    {
        ActivateTexture(textures[i]);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1.0f, 1.0f);    glVertex3f( 1.0f, y1, 0.0f);
            glTexCoord2f(0.0f, 1.0f);    glVertex3f(-1.0f, y1, 0.0f);
            glTexCoord2f(1.0f, 0.0f);    glVertex3f( 1.0f, y2, 0.0f);
            glTexCoord2f(0.0f, 0.0f);    glVertex3f(-1.0f, y2, 0.0f);

        glEnd(); 
        
        y1 = y2;
        y2 = y1 + strip_height;
    }

}

void Draw()
{
    PAL::Image left, right, disparity;
    if(PAL::GrabFrames(&left, &right, 0, &disparity) == PAL::SUCCESS)
    {
        UpdateTexture(g_texLeft, left.cols, left.rows, left.Raw.u8_data, g_pTempBuffer);
        UpdateTexture(g_texRight, right.cols, right.rows, right.Raw.u8_data, g_pTempBuffer);
        UpdateDisparity(g_texDisparity, disparity.cols, disparity.rows, disparity.Raw.u8_data, g_pTempBuffer);
    }
	
    glClearColor(0.1f, 0.1f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	GLuint textures[] = { g_texDisparity, g_texLeft, g_texRight };
    DrawStrips(textures, 3);
        
    glutSwapBuffers();
}



int main(int argc, char** argv)
{

    const int left = 0;
    const int top = 0;
    const char* windowTitle = "PAL + OpenGL combination";

    int image_width = -1, image_height = -1;
    if(PAL::Init(image_width, image_height) != PAL::SUCCESS)
    {
        printf("Failed to initialize the camera\n");
        return 1;
    }

    PAL::CameraProperties prop;
    PAL::LoadProperties("../Explorer/SavedPalProperties.txt", &prop);

   PAL::CameraProperties current_prop;
   PAL::GetCameraProperties(&current_prop);


	current_prop.computation = PAL::HIGH_QUALITY_A;
	unsigned int flags = PAL::DISPARITY_COMPUTATION;
	PAL::SetCameraProperties(&current_prop,&flags);

    g_pTempBuffer = new unsigned char[image_width*image_height*3];
    printf("Click inside the window - to toggle between BGR and RGB modes\n");
    printf("Press ESC key to close the window\n");
    printf("Press W to increase the depth display scale\n");
    printf("Press S to decrease the depth display scale\n");
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(image_width/4, image_height*3/4); 
    glutInitWindowPosition(left, top);
    g_glutWindow = glutCreateWindow(windowTitle);
    
    glutDisplayFunc(Draw);
    glutIdleFunc(Draw);

    glutReshapeFunc(OnWindowResize);
    glutKeyboardFunc(OnKeyPress);
    glutMouseFunc(OnMouseClick);
    
    g_texLeft = CreateTexture(image_width, image_height, 3, 0);
    g_texRight = CreateTexture(image_width, image_height, 3, 0);
    g_texDisparity = CreateTexture(image_width, image_height, 3, 0);

    glutMainLoop();

	return 0;
}


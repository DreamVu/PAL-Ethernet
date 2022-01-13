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


void UpdateTexture(GLuint handle, int width, int height, unsigned char* src, unsigned char* temp)
{
    temp = ConvertBGR2RGB(width, height, src, temp);
    glBindTexture(GL_TEXTURE_2D, handle);
    GLenum format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, temp);   
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
    
        
    PAL::Image left, right;
    if(PAL::GrabFrames(&left, &right) == PAL::SUCCESS)
    {
    
        UpdateTexture(g_texLeft, left.cols, left.rows, left.Raw.u8_data, g_pTempBuffer);
        UpdateTexture(g_texRight, right.cols, right.rows, right.Raw.u8_data, g_pTempBuffer);
    }
	
    glClearColor(0.1f, 0.1f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	GLuint textures[] = { g_texLeft, g_texRight };
    DrawStrips(textures, 2);
        
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
    
    
    PAL::Image left_, right_;
    PAL::GrabFrames(&left_, &right_);
    printf("WIDTH : %d, HEIGHT : %d", left_.cols, right_.rows);
    
    
    g_pTempBuffer = new unsigned char[image_width*image_height*3];
    printf("Click inside the window - to toggle between BGR and RGB modes\n");
    printf("Press ESC key to close the window\n");
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(image_width/4, image_height/2); 
    glutInitWindowPosition(left, top);
    g_glutWindow = glutCreateWindow(windowTitle);
    
    glutDisplayFunc(Draw);
    glutIdleFunc(Draw);

    glutReshapeFunc(OnWindowResize);
    glutKeyboardFunc(OnKeyPress);
    glutMouseFunc(OnMouseClick);
    
    g_texLeft = CreateTexture(image_width, image_height, 3, 0);
    g_texRight = CreateTexture(image_width, image_height, 3, 0);
    
    glutMainLoop();

	return 0;
}


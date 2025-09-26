//https://steward-fu.github.io/website/handheld/miyoo_a30_cpp_es_triangle.htm
//https://www.cnblogs.com/pengdonglin137/articles/12677212.html
//gcc triangle_x11.c -lGLESv2 -lEGL -lglfw
//sudo apt-get install libgles2-mesa-dev libglfw3-dev

#define USE_X11

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#ifdef USE_X11
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#else
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#endif

#define LCD_W   480
#define LCD_H   640
 
const char *vShaderSrc =
    "attribute vec4 vPosition;  \n"
    "void main()                \n"
    "{                          \n"
    "  gl_Position = vPosition; \n"
    "}                          \n";
 
const char *fShaderSrc =
    "precision mediump float;                       \n"
    "void main()                                    \n"
    "{                                              \n"
    "   gl_FragColor = vec4(0.7, 0.85, 0.97, 1.0);    \n"
    "}                                              \n"; 
 
int main(int argc, char** argv)
{
    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint pObject = 0;
    GLint compiled = 0;
    GLfloat vVertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
 

#ifdef USE_X11
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);


    window = glfwCreateWindow(LCD_W, LCD_H, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(window);
 
    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );
#else
    EGLint egl_major = 0;
    EGLint egl_minor = 0;
    EGLint num_configs = 0;
    EGLConfig configs = {0};
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,   8,  
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE,  8,  
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };
    EGLint window_attributes[] = { 
        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        EGL_NONE
    };
    EGLint const context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, &egl_major, &egl_minor);
    eglChooseConfig(display, config_attribs, &configs, 1, &num_configs);
    surface = eglCreateWindowSurface(display, configs, 0, window_attributes);
    context = eglCreateContext(display, configs, EGL_NO_CONTEXT, context_attributes);
    eglMakeCurrent(display, surface, surface, context);
#endif

    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
 
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
  
    pObject = glCreateProgram();
    glAttachShader(pObject, vShader);
    glAttachShader(pObject, fShader);
    glLinkProgram(pObject);
    glUseProgram(pObject);
 
#ifdef USE_X11
while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
#endif
    glViewport(0, 0, LCD_W, LCD_H);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
#ifdef USE_X11
    glfwSwapBuffers(window);
}
#else
    eglSwapBuffers(display, surface);
    sleep(3);
#endif
 
#ifdef USE_X11
    glfwTerminate();
#else
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
#endif
    return 0;
}


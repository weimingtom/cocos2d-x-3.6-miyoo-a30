//https://steward-fu.github.io/website/handheld/miyoo_a30_cpp_es_triangle.htm
//gcc triangle.c -lGLESv2 -lEGL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
 

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
    "   gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);    \n"
    "}                                              \n"; 
 
int main(int argc, char** argv)
{
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
    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint pObject = 0;
    GLint compiled = 0;
    GLfloat vVertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
 
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, &egl_major, &egl_minor);
    eglChooseConfig(display, config_attribs, &configs, 1, &num_configs);
    surface = eglCreateWindowSurface(display, configs, 0, window_attributes);
    context = eglCreateContext(display, configs, EGL_NO_CONTEXT, context_attributes);
    eglMakeCurrent(display, surface, surface, context);
 
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
 
    glViewport(0, 0, LCD_W, LCD_H);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(display, surface);
    sleep(3);
 
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    return 0;
}

# cocos2d-x-3.6-miyoo-a30
[WIP] Cocos2d-x-3.6 MIYOO A30, TRIMUI SMART PRO, TRIMUI BRICK port, now only support video output with GLES2, there is no input and audio

## About OpenGL ES 2.0 initialization code  
* Inspired by and thanks to Steward.  
* see also https://steward-fu.github.io/website/handheld/miyoo_a30_cpp_es_triangle.htm  
```
參考資訊：
https://www.khronos.org/assets/uploads/books/openglr_es_20_programming_guide_sample.pdf  
```

## Original repo and readme.md
* https://github.com/cocos2d/cocos2d-x/tree/cocos2d-x-3.6
* https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-3.6/README.md
* https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-3.6/README.cmake  

## TODO: Implement input function, do_capture  
* https://github.com/weimingtom/xiaozhi_playground/blob/master/qt4-nes_with_input/qt4-nes_v12_button_input_success.tar.gz  
evtest tiny version, see qt4-nes_v12_button_input_success.tar.gz\qt4-nes\Qt\NesEmulateWindow.cpp  

## TODO: implement input function, void GLViewImpl::pollEvents()
* https://github.com/weimingtom/kirikiroid2-miyoo-a30/blob/master/cocos/platform/desktop/CCGLViewImpl-desktop.cpp#L903
* https://github.com/yangosoft/cocos2d-x-arm-opengles/blob/master/platform/desktop/CCGLViewImpl-desktop.cpp#L826  
* NOTE, my kirikiroid2-miyoo-a30 code causes 2 windows. So please refer to PPSSPP's source, and reduce it to 1 window, and try to pass window handle to EGL/GLES2 initialization codes and functions      

## TODO: disable kirikiroid2-miyoo-a30 debug code by default  

## TODO
* https://github.com/CeZQ/dmgame   
dmgame.rar  
* http://www.cocos2d-lua.org/download/index.md  
https://github.com/u0u0/Quick-Cocos2dx-Community/blob/master/quick/template/src/main.lua    
Quick-Cocos2dx-Community  
Quick-Cocos2dx-Community_3.6_release_190dccc.zip  
* Doudizhu.zip  
萌妹斗地主_cocos2dx_scut, client, Cocos2d-x游戏《萌妹斗地主》客户端    
https://github.com/ScutGame/Client-source/tree/master  
* https://github.com/meetleev/CocosGui/tree/master  
guiForLua.rar  
* cocos2d-x3.6 连连看完整源码   
* Cocos2d-x 3.x 案例开发大全（第2版）    
* https://github.com/yangosoft/cocos2d-x-arm-opengles  

## How to Build  
* For PC, xubuntu 20.04 64bit  
libbz2-dev isn't necessary, only for cross compiling  
```
sudo apt install libglfw3-dev
sudo apt install libfreetype-dev
sudo apt install libjpeg-dev
sudo apt install libbz2-dev
sudo apt install make gcc g++ gdb

make MIYOO=0 clean
make MIYOO=0 -j8
make test
```
* For TRIMUI SMART PRO  
```
cd /home/wmt/work_trimui/
tar xzf aarch64-linux-gnu-7.5.0-linaro.tgz 
tar xzf SDK_usr_tg5040_a133p.tgz

cd /home/wmt/work_cocos2dx/cocos2d-x-3.6-miyoo-a30/
make MIYOO=2 clean
make MIYOO=2 -j8
file tests/cpp-empty-test/cpp-empty-test
```

## How to build original version Cocos2d-x 3.6   
* sudo apt install libglfw3-dev
* comment the last line of build/install_deps_linux.sh  
* cd build  
* ./install_deps_linux.sh  
* cmake ..  
* Modify cocos/platform/linux/CCStdC-Linux.h   
```
#include <math.h>  
改成  
#include <cmath>  
using std::isnan;  
```
```
在ubuntu 16上编译运行cocos2d-x 3.6的效果，
只能说要改一些东西，需要装cmake和libglfw3-dev，
最麻烦的是glfw3，因为ubuntu 14很难cmake编译安装glfw3，
所以需要用ubuntu 16或以上，
另外glfw3需要较旧的版本3.0.4，
这里直接用apt安装即可。
编译方法是先执行cocos2d-x 2的install-deps-linux.sh，
然后apt安装libglfw3-dev（它会把libglfw-dev自动卸载），
然后就可以成功cmake了。
执行make后可能会有编译错误，需要在CCStdC-linux.h
里面加上include <cmath>和using std::isnan;
```
* Copy cocos/editor-support/cocostudio to cocos/ for C++ include path  
* cmake min: cocos2d-x-3.6_v3.tar.gz  
* cmake min with csb: cocos2d-x-3.6_v3.1.tar.gz  
* make min: cocos2d-x-3.6_v5.tar.gz  
* GLES2 merge back to pc: cocos2d-x-3.6_v3.5_merge_back_pc.tar.gz    

## Test GLES+X11 and GLES+NoX11
* testgles_20241128.tar.gz
* (not solved) OpenGL error 0x0506 CCRenderer
* (not solved) GL_INVALID_FRAMEBUFFER_OPERATION
* If run GLES+NoX11 code under X11, it will show: libEGL warning: DRI2 :failed to authenticate

## Msys2 Ref  
* cocos2d-x-3.6_msys2_v1_good.zip  
* NOTE, some codes are not merged   

## Macos Ref  
* macos_build_out.zip  
* (TODO) remove !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX) && !defined(__APPLE__), remove TVPGL_ASM_Test
* (TODO) remove !defined( _MSC_VER) && !defined(LINUX) && !defined(__APPLE__)
* (TODO) remove defined(_MSC_VER) || defined(ANDROID) || defined(LINUX) || defined(__APPLE__)
* (TODO) remove defined(_MSC_VER) || defined(LINUX) || defined(__APPLE__), 4 places
* (TODO, need to reopen and test) 0 //TODO, remove when macos
* see upper, !defined(__APPLE__) //TODO, remove when macos
* renderer/CCTextureAtlas.cpp, renderer/CCRenderer.cpp, defined(__APPLE__)
* cocos2dx_1_v2.zip macos test code

## About running on VisionFive2
* Make sure tempture lower than 85 degree and using a CPU fan  
* see https://www.waveshare.net/wiki/VisionFive2  
```
上次说的visionfive2另外一个问题是u口容易失灵，
不过我后来发现可能这不是visionfive2的硬件问题，
而是它温度太高的时候可能会关掉一些功能（微雪文档说超过85度就会降频），
所以最好用风扇吹着（我是拿树莓派4风扇接上去靠边的第2第3针脚）。
不过即便如此有时候u口也会失灵，如果可以的话最好还是用
lftp网线传而不是用u盘。所以这个visionfive2还不是很适合用于
一些对稳定要求很高的场景，用来玩倒是没太大问题
```
* How to Use Hardware GPU accelerated GL_RENDERER, instead of softpipe MESA software OpenGLES  
* [1] First, use -lGLESv2_PVR_MESA instead of -lGLESv2  
* [2] Second, use cmake build static library libglfw3.a instead of apt install libglfw3-dev  
* [3] Third, use GLFW_OPENGL_ES_API  
* See SDL_VIDEO_GL_DRIVER='GLESv2_PVR_MESA', see https://forum.rvspace.org/t/play-psp-games-on-visionfive-2/3525  
```
visionfive2的opengl卡顿问题我似乎找到方法解决了，
效果如下，GL_RENDERER显示不是softpipe而是PowerVR B-Series BXE-4-32。
如何用硬件GPU而避免softpipe软件实现的OpenGLES呢？很多人都没说，
我只能在PPSSPP移植到visionfive2的作者那里找到头绪：

（1）链接到动态库-lGLESv2_PVR_MESA而不是-lGLESv2，至于-lEGL不用变
（2）glfw3，以及其他和GLES有关的库最好自己编译成静态库，不要用apt install的库，
避免链接到软件实现mesa动态库
（3）代码里面要明确使用opengles而不是opengl，
例如使用glfw3时要加上GLFW_OPENGL_ES_API这个hint，否则窗口返回空
```
* cocos2d-x-3.6-miyoo-a30_vf2_v2_hw_gles.tar.gz
* **FIXME**: -g0 -O3 link failed
* (**TODO**) Use which firmware
* glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
* DMY_USE_GLES_GLFW3
* You can use strace to test GL_RENDERER=softpipe problem (should load libGLESv2_PVR_MESA.so instead of libGLESv2.so), or to analyze PPSSPP visionfive2 port building

## About building GLEW+GLFW PC version, just for MIYOO=0 
* see USEGLEW
* make USEGLEW=1 clean
* make USEGLEW=1 -j8

## Steam Deck Ubuntu 25.04 build
```
我测试过，如果用steam deck的tf卡启动ubuntu 25.04的livecd模式（rufus烧录），
它的glew+glfw3是会自带AMD GPU驱动的，不需要自己装显卡驱动，
只要自己写的代码使用了glew和glfw3的dev软件包即可。
如图，我是用cocos2d-x输出renderer信息，或者用
mesa-utils的glxinfo | grep renderer
也可以看到使用了硬件显卡驱动。还有一种方法是
自己写一段代码去输出GL_RENDERER信息
（和cocos2d-x的输出渲染器信息是一样的方法）
```

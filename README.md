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
* https://github.com/weimingtom/cocos2d-x-3.6-miyoo-a30/blob/master/README_old.md
* https://github.com/weimingtom/cocos2d-x-3.6-miyoo-a30/blob/master/README.cmake

## TODO: Implement input function, do_capture  
* https://github.com/weimingtom/xiaozhi_playground/blob/master/qt4-nes_with_input/qt4-nes_v12_button_input_success.tar.gz  
evtest tiny version, see qt4-nes_v12_button_input_success.tar.gz\qt4-nes\Qt\NesEmulateWindow.cpp  

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


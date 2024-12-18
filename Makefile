#0: pc
#1: miyoo a30 and rg28xx
#2: trimui smart pro
#3: trimui brick
MIYOO := 1

ifeq ($(MIYOO),3)
CC = /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-gcc
CPP := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-g++
AR := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ar cru
RANLIB := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ranlib 
else ifeq ($(MIYOO),2)
CC = /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-gcc
CPP := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-g++
AR := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ar cru
RANLIB := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ranlib 
else ifeq ($(MIYOO),1)
CC = /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CPP := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
AR := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-ar cru
RANLIB := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-ranlib 
else
CC  := gcc
CPP := g++
AR  := ar cru
RANLIB := ranlib
endif
RM := rm -rf

CPPFLAGS := 
CPPFLAGS += -I.   
#CPPFLAGS += -g -O2
CPPFLAGS += -g3 -O0

ifeq ($(MIYOO),3)
#trimui brick
CPPFLAGS += -DUSE_APP_WIDTH=1024
CPPFLAGS += -DUSE_APP_HEIGHT=768
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
else ifeq ($(MIYOO),2)
#trimui smart pro
CPPFLAGS += -DUSE_APP_WIDTH=1280
CPPFLAGS += -DUSE_APP_HEIGHT=720
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
else ifeq ($(MIYOO),1)
#miyoo a30 and rg28xx
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=1
else
#pc
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480
CPPFLAGS += -DUSE_NO_GLFW=0 -DUSE_SHADER_PRECISION=1
#if PC use OpenGLES to replace OpenGL, need USE_SHADER_PRECISION
#cocos2d-x debug info [cocos2d: 0:1(1): error: No precision specified in this scope for type `mat4'
CPPFLAGS += -DUSE_ROTATE90=0
endif

CPPFLAGS += -DCC_ENABLE_BOX2D_INTEGRATION=0 
CPPFLAGS += -DCC_USE_PHYSICS=0 

#CPPFLAGS += -DCC_USE_WEBP=1
CPPFLAGS += -DCC_USE_WEBP=0
CPPFLAGS += -DCC_USE_TIFF=0
#if USE_FONTCONFIG_LIB

CPPFLAGS += -DGLFW_DLL 
CPPFLAGS += -DLINUX 
CPPFLAGS += -DCOCOS2D_DEBUG=1

#see below
CPPFLAGS2 := 
CPPFLAGS2 += -std=c++11 

CPPFLAGS += -fno-exceptions 
CPPFLAGS += -fexceptions 

CPPFLAGS += -Wno-deprecated-declarations 
CPPFLAGS += -Wno-reorder 
CPPFLAGS += -Wall 

CPPFLAGS += -I. 
CPPFLAGS += -Icocos 
CPPFLAGS += -Icocos/platform 
CPPFLAGS += -Icocos/platform/desktop 
CPPFLAGS += -Icocos/audio/include 
CPPFLAGS += -Icocos/editor-support
CPPFLAGS += -Ideprecated 
CPPFLAGS += -Iextensions 

CPPFLAGS += -Iexternal 
CPPFLAGS += -Iexternal/ConvertUTF 
CPPFLAGS += -Iexternal/poly2tri 
CPPFLAGS += -Iexternal/poly2tri/common 
CPPFLAGS += -Iexternal/poly2tri/sweep 
CPPFLAGS += -Iexternal/edtaa3func 
CPPFLAGS += -Iexternal/xxtea 
CPPFLAGS += -Iexternal/unzip 
CPPFLAGS += -Iexternal/tinyxml2 
CPPFLAGS += -Iexternal/xxhash 

#CPPFLAGS += -I/usr/include/webp
#CPPFLAGS += -I/usr/include/i386-linux-gnu 
#CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/GLFW 
#CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/local/include/GLFW 

ifeq ($(MIYOO),3)
CPPFLAGS += -I/home/wmt/work_trimui/usr/include
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/webp
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/freetype2
else ifeq ($(MIYOO),2)
CPPFLAGS += -I/home/wmt/work_trimui/usr/include
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/webp
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/freetype2
else ifeq ($(MIYOO),1)
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/webp
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/freetype2
else
#xubuntu 16
#sudo apt install libglfw3-dev libpng-dev zlib1g-dev libjpeg-dev libfreetype-dev libbz2-dev
CPPFLAGS += -I/usr/include
CPPFLAGS += -I/usr/include/webp
CPPFLAGS += -I/usr/include/freetype2
endif



LDFLAGS := 
#LDFLAGS += -lGLU 

#LDFLAGS += -lGL 
#LDFLAGS += -lGLEW 
#LDFLAGS += -lglfw 
#LDFLAGS += -lfontconfig 
#LDFLAGS += -ltiff
#LDFLAGS += -lwebp
#LDFLAGS += -lwebsockets 
#LDFLAGS += -lcurl 
#LDFLAGS += -lX11 

LDFLAGS += -lfreetype -lbz2 
LDFLAGS += -lpthread 
LDFLAGS += -lz 
LDFLAGS += -ljpeg 
LDFLAGS += -lpng 
LDFLAGS += -lGLESv2 -lEGL

ifeq ($(MIYOO),3)
#trimui brick replace glfw3 with EGL
LDFLAGS += -lIMGegl -lsrv_um -lusc -lglslcompiler -L/home/wmt/work_trimui/usr/lib
else ifeq ($(MIYOO),2)
#trimui smart pro replace glfw3 with EGL
LDFLAGS += -lIMGegl -lsrv_um -lusc -lglslcompiler -L/home/wmt/work_trimui/usr/lib
else ifeq ($(MIYOO),1)
#miyoo a30 replace glfw3 with EGL
LDFLAGS += -L/home/wmt/work_a30/staging_dir/target/usr/lib
else
#xubuntu 16, need glfw3 and EGL
#sudo apt install libglfw3-dev libpng-dev zlib1g-dev libjpeg-dev libfreetype-dev libbz2-dev
LDFLAGS += -lglfw #cocos2dx/platform/linux/CCEGLView.cpp
endif


OBJS := 

#libcocos2d.a
OBJS += cocos/cocos2d.o

#2d
OBJS += cocos/2d/CCAction.o
OBJS += cocos/2d/CCActionCamera.o
OBJS += cocos/2d/CCActionCatmullRom.o
OBJS += cocos/2d/CCActionEase.o
OBJS += cocos/2d/CCActionGrid.o
OBJS += cocos/2d/CCActionGrid3D.o
OBJS += cocos/2d/CCActionInstant.o
OBJS += cocos/2d/CCActionInterval.o
OBJS += cocos/2d/CCActionManager.o
OBJS += cocos/2d/CCActionPageTurn3D.o
OBJS += cocos/2d/CCActionProgressTimer.o
OBJS += cocos/2d/CCActionTiledGrid.o
OBJS += cocos/2d/CCActionTween.o
OBJS += cocos/2d/CCAnimation.o
OBJS += cocos/2d/CCAnimationCache.o
OBJS += cocos/2d/CCAtlasNode.o
OBJS += cocos/2d/CCCamera.o
OBJS += cocos/2d/CCClippingNode.o
OBJS += cocos/2d/CCClippingRectangleNode.o
OBJS += cocos/2d/CCComponent.o
OBJS += cocos/2d/CCComponentContainer.o
OBJS += cocos/2d/CCDrawNode.o
OBJS += cocos/2d/CCDrawingPrimitives.o
OBJS += cocos/2d/CCFont.o
OBJS += cocos/2d/CCFontAtlas.o
OBJS += cocos/2d/CCFontAtlasCache.o
OBJS += cocos/2d/CCFontCharMap.o
OBJS += cocos/2d/CCFontFNT.o
OBJS += cocos/2d/CCFontFreeType.o
OBJS += cocos/2d/CCGLBufferedNode.o
OBJS += cocos/2d/CCGrabber.o
OBJS += cocos/2d/CCGrid.o
OBJS += cocos/2d/CCLabel.o
OBJS += cocos/2d/CCLabelAtlas.o
OBJS += cocos/2d/CCLabelBMFont.o
OBJS += cocos/2d/CCLabelTTF.o
OBJS += cocos/2d/CCLabelTextFormatter.o
OBJS += cocos/2d/CCLayer.o
OBJS += cocos/2d/CCLight.o
OBJS += cocos/2d/CCMenu.o
OBJS += cocos/2d/CCMenuItem.o
OBJS += cocos/2d/CCMotionStreak.o
OBJS += cocos/2d/CCNode.o
OBJS += cocos/2d/CCNodeGrid.o
OBJS += cocos/2d/CCParallaxNode.o
OBJS += cocos/2d/CCParticleBatchNode.o
OBJS += cocos/2d/CCParticleExamples.o
OBJS += cocos/2d/CCParticleSystem.o
OBJS += cocos/2d/CCParticleSystemQuad.o
OBJS += cocos/2d/CCProgressTimer.o
OBJS += cocos/2d/CCProtectedNode.o
OBJS += cocos/2d/CCRenderTexture.o
OBJS += cocos/2d/CCScene.o
OBJS += cocos/2d/CCSprite.o
OBJS += cocos/2d/CCSpriteBatchNode.o
OBJS += cocos/2d/CCSpriteFrame.o
OBJS += cocos/2d/CCSpriteFrameCache.o
OBJS += cocos/2d/MarchingSquare.o
OBJS += cocos/2d/SpritePolygon.o
OBJS += cocos/2d/SpritePolygonCache.o
OBJS += cocos/2d/CCTMXLayer.o
OBJS += cocos/2d/CCFastTMXLayer.o
OBJS += cocos/2d/CCTMXObjectGroup.o
OBJS += cocos/2d/CCTMXTiledMap.o
OBJS += cocos/2d/CCFastTMXTiledMap.o
OBJS += cocos/2d/CCTMXXMLParser.o
OBJS += cocos/2d/CCTextFieldTTF.o
OBJS += cocos/2d/CCTileMapAtlas.o
OBJS += cocos/2d/CCTransition.o
OBJS += cocos/2d/CCTransitionPageTurn.o
OBJS += cocos/2d/CCTransitionProgress.o
OBJS += cocos/2d/CCTweenFunction.o

OBJS += cocos/3d/CCFrustum.o
OBJS += cocos/3d/CCPlane.o
OBJS += cocos/platform/CCGLView.o
OBJS += cocos/platform/CCFileUtils.o
OBJS += cocos/platform/CCSAXParser.o
OBJS += cocos/platform/CCThread.o
OBJS += cocos/platform/CCImage.o

OBJS += cocos/platform/linux/CCApplication-linux.o
OBJS += cocos/platform/linux/CCCommon-linux.o
OBJS += cocos/platform/linux/CCDevice-linux.o
OBJS += cocos/platform/linux/CCFileUtils-linux.o
OBJS += cocos/platform/linux/CCStdC-linux.o

OBJS += cocos/platform/desktop/CCGLViewImpl-desktop.o

#math
OBJS += cocos/math/CCAffineTransform.o
OBJS += cocos/math/CCGeometry.o
OBJS += cocos/math/CCVertex.o
OBJS += cocos/math/MathUtil.o
OBJS += cocos/math/Mat4.o
OBJS += cocos/math/Quaternion.o
OBJS += cocos/math/TransformUtils.o
OBJS += cocos/math/Vec2.o
OBJS += cocos/math/Vec3.o
OBJS += cocos/math/Vec4.o

#base
OBJS += cocos/base/CCAsyncTaskPool.o
OBJS += cocos/base/CCAutoreleasePool.o
OBJS += cocos/base/CCConfiguration.o
OBJS += cocos/base/CCConsole.o
OBJS += cocos/base/CCData.o
OBJS += cocos/base/CCDataVisitor.o
OBJS += cocos/base/CCDirector.o
OBJS += cocos/base/CCEvent.o
OBJS += cocos/base/CCEventAcceleration.o
OBJS += cocos/base/CCEventCustom.o
OBJS += cocos/base/CCEventDispatcher.o
OBJS += cocos/base/CCEventFocus.o
OBJS += cocos/base/CCEventKeyboard.o
OBJS += cocos/base/CCEventController.o
OBJS += cocos/base/CCEventListener.o
OBJS += cocos/base/CCEventListenerController.o
OBJS += cocos/base/CCEventListenerAcceleration.o
OBJS += cocos/base/CCEventListenerCustom.o
OBJS += cocos/base/CCEventListenerFocus.o
OBJS += cocos/base/CCEventListenerKeyboard.o
OBJS += cocos/base/CCEventListenerMouse.o
OBJS += cocos/base/CCEventListenerTouch.o
OBJS += cocos/base/CCEventMouse.o
OBJS += cocos/base/CCEventTouch.o
OBJS += cocos/base/CCIMEDispatcher.o
OBJS += cocos/base/CCNS.o
OBJS += cocos/base/CCProfiling.o
OBJS += cocos/base/ccRandom.o
OBJS += cocos/base/CCRef.o
OBJS += cocos/base/CCScheduler.o
OBJS += cocos/base/CCScriptSupport.o
OBJS += cocos/base/CCTouch.o
OBJS += cocos/base/CCUserDefault.o
#OBJS += cocos/base/CCUserDefault-android.o
OBJS += cocos/base/CCValue.o
OBJS += cocos/base/TGAlib.o
OBJS += cocos/base/ZipUtils.o
OBJS += cocos/base/atitc.o
OBJS += cocos/base/base64.o
OBJS += cocos/base/ccCArray.o
OBJS += cocos/base/ccFPSImages.o
OBJS += cocos/base/ccTypes.o
OBJS += cocos/base/ccUTF8.o
OBJS += cocos/base/ccUtils.o
OBJS += cocos/base/etc1.o
OBJS += cocos/base/pvr.o
OBJS += cocos/base/s3tc.o
OBJS += cocos/base/CCController.o
#OBJS += cocos/base/CCController-android.o
OBJS += cocos/base/allocator/CCAllocatorDiagnostics.o
OBJS += cocos/base/allocator/CCAllocatorGlobal.o
OBJS += cocos/base/allocator/CCAllocatorGlobalNewDelete.o
OBJS += cocos/base/ObjectFactory.o

#renderer
OBJS += cocos/renderer/CCBatchCommand.o
OBJS += cocos/renderer/CCCustomCommand.o
OBJS += cocos/renderer/CCGLProgram.o
OBJS += cocos/renderer/CCGLProgramCache.o
OBJS += cocos/renderer/CCGLProgramState.o
OBJS += cocos/renderer/CCGLProgramStateCache.o
OBJS += cocos/renderer/CCGroupCommand.o
OBJS += cocos/renderer/CCQuadCommand.o
OBJS += cocos/renderer/CCMeshCommand.o
OBJS += cocos/renderer/CCRenderCommand.o
OBJS += cocos/renderer/CCRenderer.o
OBJS += cocos/renderer/CCTexture2D.o
OBJS += cocos/renderer/CCTextureAtlas.o
OBJS += cocos/renderer/CCTextureCache.o
OBJS += cocos/renderer/ccGLStateCache.o
OBJS += cocos/renderer/ccShaders.o
OBJS += cocos/renderer/CCVertexIndexBuffer.o
OBJS += cocos/renderer/CCVertexIndexData.o
OBJS += cocos/renderer/CCPrimitive.o
OBJS += cocos/renderer/CCPrimitiveCommand.o
OBJS += cocos/renderer/CCTrianglesCommand.o

#deprecated
OBJS += cocos/deprecated/CCArray.o
OBJS += cocos/deprecated/CCSet.o
OBJS += cocos/deprecated/CCString.o
OBJS += cocos/deprecated/CCDictionary.o
OBJS += cocos/deprecated/CCDeprecated.o
OBJS += cocos/deprecated/CCNotificationCenter.o

OBJS += external/ConvertUTF/ConvertUTFWrapper.o
OBJS += external/ConvertUTF/ConvertUTF.o

OBJS += external/edtaa3func/edtaa3func.o

OBJS += external/poly2tri/common/shapes.o
OBJS += external/poly2tri/sweep/advancing_front.o
OBJS += external/poly2tri/sweep/cdt.o
OBJS += external/poly2tri/sweep/sweep_context.o
OBJS += external/poly2tri/sweep/sweep.o

#LDFLAGS += ../../lib/libtinyxml2.a 
OBJS += external/tinyxml2/tinyxml2.o

#LDFLAGS += ../../lib/libunzip.a 
OBJS += external/unzip/ioapi_mem.o
OBJS += external/unzip/ioapi.o
OBJS += external/unzip/unzip.o

#LDFLAGS += ../../lib/libxxhash.a 
OBJS += external/xxhash/xxhash.o

HELLOCPP_OBJS :=
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/AppDelegate.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/HelloWorldScene.o

all : tests/cpp-empty-test/cpp-empty-test

cocos2dx.a : $(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

tests/cpp-empty-test/cpp-empty-test: cocos2dx.a $(HELLOCPP_OBJS)
	$(CPP) tests/cpp-empty-test/proj.linux/main.cpp $(HELLOCPP_OBJS) cocos2dx.a -o $@ -Itests/cpp-empty-test/Classes $(CPPFLAGS) $(LDFLAGS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.cc
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.c
	$(CC) $(CPPFLAGS) -o $@ -c $<
	
test:
	./tests/cpp-empty-test/cpp-empty-test

clean :
	$(RM) $(OBJS) $(HELLOCPP_OBJS) cocos2dx.a tests/cpp-empty-test/cpp-empty-test
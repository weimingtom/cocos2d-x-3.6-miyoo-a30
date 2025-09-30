#0: pc
#1: miyoo a30 and rg28xx
#2: trimui smart pro
#3: trimui brick
MIYOO := 0

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
#CPPFLAGS += -g3 -O0

ifeq ($(MIYOO),3)
#trimui brick
CPPFLAGS += -DUSE_APP_WIDTH=1024
CPPFLAGS += -DUSE_APP_HEIGHT=768
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g0 -O3
else ifeq ($(MIYOO),2)
#trimui smart pro
CPPFLAGS += -DUSE_APP_WIDTH=1280
CPPFLAGS += -DUSE_APP_HEIGHT=720
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g0 -O3
else ifeq ($(MIYOO),1)
#miyoo a30 and rg28xx
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=1
CPPFLAGS += -g0 -O3
else
#pc
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480

#sudo apt install libglew-dev libglfw3-dev
USEGLEW:=0

ifeq ($(USEGLEW),1)
#opengl on x11
CPPFLAGS += -DUSE_GLEW=1 -DUSE_SHADER_PRECISION=1 
#-DGLEW_STATIC not need under linux
else
#gles on x11
CPPFLAGS += -DUSE_NO_GLFW=0 -DUSE_SHADER_PRECISION=1
endif

#if PC use OpenGLES to replace OpenGL, need USE_SHADER_PRECISION
#cocos2d-x debug info [cocos2d: 0:1(1): error: No precision specified in this scope for type `mat4'
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g3 -O0
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

CPPFLAGS += -DMY_USE_FREESERIF=1
CPPFLAGS += -DMY_USE_UIBUTTON_SETTITLECOLOR=1

#see below
CPPFLAGS2 := 
CPPFLAGS2 += -std=c++11 

CPPFLAGS += -fno-exceptions 
CPPFLAGS += -fexceptions 

CPPFLAGS += -Wno-deprecated-declarations 
#only c:
#CPPFLAGS += -Wno-reorder 
CPPFLAGS += -Wall 

CPPFLAGS += -I. 
CPPFLAGS += -Icocos 
CPPFLAGS += -Icocos/platform 
#FIXME:
CPPFLAGS += -Icocos/platform/desktop 
CPPFLAGS += -Icocos/platform/linux 
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

CPPFLAGS += -Icocos/ui
CPPFLAGS += -Icocos/ui/UIEditBox
CPPFLAGS += -Icocos/editor-support/cocosbuilder
CPPFLAGS += -Icocos/editor-support/cocostudio
CPPFLAGS += -Icocos/editor-support/cocostudio/ActionTimeline
CPPFLAGS += -Icocos/editor-support/cocostudio/WidgetReader

CPPFLAGS += -Itests/cpp-empty-test/Classes
CPPFLAGS += -Itests/cpp-empty-test/Classes/cocos2d

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
#xubuntu 20
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
#NOTE: -lbz2 is not necessary for PC xubuntu 20.04
LDFLAGS += -lpthread 
LDFLAGS += -lz 
LDFLAGS += -ljpeg 
LDFLAGS += -lpng 
ifeq ($(USEGLEW),1)
LDFLAGS += -lGL
else
LDFLAGS += -lGLESv2 -lEGL
endif

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
ifeq ($(USEGLEW),1)
LDFLAGS += -lglfw -lGLEW #cocos2dx/platform/linux/CCEGLView.cpp
else
LDFLAGS += -lglfw #cocos2dx/platform/linux/CCEGLView.cpp
endif

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

#ui
OBJS += cocos/ui/UIWidget.o
OBJS += cocos/ui/UILayout.o
OBJS += cocos/ui/UILayoutParameter.o
OBJS += cocos/ui/UILayoutManager.o
OBJS += cocos/ui/CocosGUI.o
OBJS += cocos/ui/UIHelper.o
OBJS += cocos/ui/UIListView.o
OBJS += cocos/ui/UIPageView.o
OBJS += cocos/ui/UIScrollView.o
OBJS += cocos/ui/UIButton.o
OBJS += cocos/ui/UICheckBox.o
OBJS += cocos/ui/UIImageView.o
OBJS += cocos/ui/UIText.o
OBJS += cocos/ui/UITextAtlas.o
OBJS += cocos/ui/UITextBMFont.o
OBJS += cocos/ui/UILoadingBar.o
OBJS += cocos/ui/UISlider.o
OBJS += cocos/ui/UITextField.o
OBJS += cocos/ui/UIRichText.o
OBJS += cocos/ui/UIHBox.o
OBJS += cocos/ui/UIVBox.o
OBJS += cocos/ui/UIRelativeBox.o
OBJS += cocos/ui/UIVideoPlayer-android.o
OBJS += cocos/ui/UIDeprecated.o
OBJS += cocos/ui/UIScale9Sprite.o
OBJS += cocos/ui/UIWebView.o
OBJS += cocos/ui/UIWebViewImpl-android.o
OBJS += cocos/ui/UIEditBox/UIEditBox.o
OBJS += cocos/ui/UIEditBox/UIEditBoxImpl-android.o
OBJS += cocos/ui/UILayoutComponent.o

#cocosbuilder
OBJS += cocos/editor-support/cocosbuilder/CCBAnimationManager.o
OBJS += cocos/editor-support/cocosbuilder/CCBFileLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCBKeyframe.o
OBJS += cocos/editor-support/cocosbuilder/CCBReader.o
OBJS += cocos/editor-support/cocosbuilder/CCBSequence.o
OBJS += cocos/editor-support/cocosbuilder/CCBSequenceProperty.o
OBJS += cocos/editor-support/cocosbuilder/CCControlButtonLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCControlLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLabelBMFontLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLabelTTFLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerColorLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerGradientLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCMenuItemImageLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCMenuItemLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCNode+CCBRelativePositioning.o
OBJS += cocos/editor-support/cocosbuilder/CCNodeLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCNodeLoaderLibrary.o
OBJS += cocos/editor-support/cocosbuilder/CCParticleSystemQuadLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCScale9SpriteLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCScrollViewLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCSpriteLoader.o

#cocosstudio
OBJS += cocos/editor-support/cocostudio/CCActionFrame.o
OBJS += cocos/editor-support/cocostudio/CCActionFrameEasing.o
OBJS += cocos/editor-support/cocostudio/CCActionManagerEx.o
OBJS += cocos/editor-support/cocostudio/CCActionNode.o
OBJS += cocos/editor-support/cocostudio/CCActionObject.o
OBJS += cocos/editor-support/cocostudio/CCArmature.o
OBJS += cocos/editor-support/cocostudio/CCBone.o
OBJS += cocos/editor-support/cocostudio/CCArmatureAnimation.o
OBJS += cocos/editor-support/cocostudio/CCProcessBase.o
OBJS += cocos/editor-support/cocostudio/CCTween.o
OBJS += cocos/editor-support/cocostudio/CCDatas.o
OBJS += cocos/editor-support/cocostudio/CCBatchNode.o
OBJS += cocos/editor-support/cocostudio/CCDecorativeDisplay.o
OBJS += cocos/editor-support/cocostudio/CCDisplayFactory.o
OBJS += cocos/editor-support/cocostudio/CCDisplayManager.o
OBJS += cocos/editor-support/cocostudio/CCSkin.o
OBJS += cocos/editor-support/cocostudio/CCColliderDetector.o
OBJS += cocos/editor-support/cocostudio/CCArmatureDataManager.o
OBJS += cocos/editor-support/cocostudio/CCArmatureDefine.o
OBJS += cocos/editor-support/cocostudio/CCDataReaderHelper.o
OBJS += cocos/editor-support/cocostudio/CCSpriteFrameCacheHelper.o
OBJS += cocos/editor-support/cocostudio/CCTransformHelp.o
OBJS += cocos/editor-support/cocostudio/CCUtilMath.o
OBJS += cocos/editor-support/cocostudio/CCComAttribute.o
OBJS += cocos/editor-support/cocostudio/CCComAudio.o
OBJS += cocos/editor-support/cocostudio/CCComController.o
OBJS += cocos/editor-support/cocostudio/CCComRender.o
OBJS += cocos/editor-support/cocostudio/CCInputDelegate.o
OBJS += cocos/editor-support/cocostudio/DictionaryHelper.o
OBJS += cocos/editor-support/cocostudio/CCSGUIReader.o
OBJS += cocos/editor-support/cocostudio/CCSSceneReader.o
OBJS += cocos/editor-support/cocostudio/TriggerBase.o
OBJS += cocos/editor-support/cocostudio/TriggerMng.o
OBJS += cocos/editor-support/cocostudio/TriggerObj.o
OBJS += cocos/editor-support/cocostudio/CocoLoader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/NodeReader/NodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SingleNodeReader/SingleNodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SpriteReader/SpriteReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ParticleReader/ParticleReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/GameMapReader/GameMapReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ProjectNodeReader/ProjectNodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ComAudioReader/ComAudioReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/WidgetReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ButtonReader/ButtonReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/CheckBoxReader/CheckBoxReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ImageViewReader/ImageViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/LayoutReader/LayoutReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ListViewReader/ListViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/LoadingBarReader/LoadingBarReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/PageViewReader/PageViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ScrollViewReader/ScrollViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SliderReader/SliderReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextAtlasReader/TextAtlasReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextBMFontReader/TextBMFontReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextFieldReader/TextFieldReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextReader/TextReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/Node3DReader/Node3DReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/Sprite3DReader/Sprite3DReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/UserCameraReader/UserCameraReader.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimelineCache.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCFrame.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCTimeLine.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimeline.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimelineNode.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CSLoader.o
OBJS += cocos/editor-support/cocostudio/FlatBuffersSerialize.o
OBJS += cocos/editor-support/cocostudio/WidgetCallBackHandlerProtocol.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ArmatureNodeReader/ArmatureNodeReader.o
OBJS += cocos/editor-support/cocostudio/CCObjectExtensionData.o
OBJS += cocos/editor-support/cocostudio/CocoStudio.o

#3d
OBJS += cocos/3d/CCRay.o
OBJS += cocos/3d/CCAABB.o
OBJS += cocos/3d/CCOBB.o
OBJS += cocos/3d/CCAnimate3D.o
OBJS += cocos/3d/CCAnimation3D.o
OBJS += cocos/3d/CCAttachNode.o
OBJS += cocos/3d/CCBillBoard.o
OBJS += cocos/3d/CCBundle3D.o
OBJS += cocos/3d/CCBundleReader.o
OBJS += cocos/3d/CCMesh.o
OBJS += cocos/3d/CCMeshSkin.o
OBJS += cocos/3d/CCMeshVertexIndexData.o
OBJS += cocos/3d/CCSprite3DMaterial.o
OBJS += cocos/3d/CCObjLoader.o
OBJS += cocos/3d/CCSkeleton3D.o
OBJS += cocos/3d/CCSprite3D.o
OBJS += cocos/3d/CCTerrain.o
OBJS += cocos/3d/CCSkybox.o
OBJS += cocos/3d/CCTextureCube.o

#extensions ScrollView
OBJS += extensions/GUI/CCControlExtension/CCControl.o
OBJS += extensions/GUI/CCControlExtension/CCControlButton.o
OBJS += extensions/GUI/CCControlExtension/CCControlColourPicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlHuePicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlPotentiometer.o
OBJS += extensions/GUI/CCControlExtension/CCControlSaturationBrightnessPicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlSlider.o
OBJS += extensions/GUI/CCControlExtension/CCControlStepper.o
OBJS += extensions/GUI/CCControlExtension/CCControlSwitch.o
OBJS += extensions/GUI/CCControlExtension/CCControlUtils.o
OBJS += extensions/GUI/CCControlExtension/CCInvocation.o
OBJS += extensions/GUI/CCScrollView/CCScrollView.o
OBJS += extensions/GUI/CCScrollView/CCTableView.o
OBJS += extensions/GUI/CCScrollView/CCTableViewCell.o

HELLOCPP_OBJS :=
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/AppDelegate.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/HelloWorldScene.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/cocos2d/MainScene.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/BaseForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/ConsoleWindow.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/DebugViewLayerForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/FileSelectorForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/GameMainMenu.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/GlobalPreferenceForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/IndividualPreferenceForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/InGameMenuForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/MainFileSelectorForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/MessageBox.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/PreferenceForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/SeletListForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/SimpleMediaFilePlayer.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/TipsHelpForm.o
HELLOCPP_OBJS += tests/cpp-empty-test/Classes/ui/XP3RepackForm.o

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

debug:
	gdb ./tests/cpp-empty-test/cpp-empty-test

#b Button::setTitleColor

clean :
	$(RM) $(OBJS) $(HELLOCPP_OBJS) cocos2dx.a tests/cpp-empty-test/cpp-empty-test

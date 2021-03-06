LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/ui)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := oncolour/main.cpp \
                   ../../Classes/Global.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/scenes/SplashScene.cpp \
                   ../../Classes/scenes/StartupScene.cpp \
                   ../../Classes/scenes/GameScene.cpp \
                   ../../Classes/scenes/PreferenceScene.cpp \
                   ../../Classes/scenes/TutorialScene.cpp \
                   ../../Classes/scenes/AboutScene.cpp \
                   ../../Classes/widgets/MenuItemLabelTint.cpp \
                   ../../Classes/widgets/Bubble.cpp \
                   ../../Classes/widgets/ToggleBubble.cpp \
                   ../../Classes/widgets/BorderedBubble.cpp \
                   ../../Classes/widgets/Photon.cpp \
                   ../../Classes/widgets/SineVelPhoton.cpp \
                   ../../Classes/widgets/Bomb.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/scenes \
                    $(LOCAL_PATH)/../../Classes/widgets

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
$(call import-module,ui)

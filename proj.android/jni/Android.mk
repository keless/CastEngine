LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_CFLAGS +=  -DOUYA=1

CPP_DIR_ZZ := $(wildcard $(LOCAL_PATH)/../../Classes/ZZ/*.cpp)
CPP_DIR_CE := $(wildcard $(LOCAL_PATH)/../../Classes/CastEngine/*.cpp)
CPP_DIR_CLASSES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)

LOCAL_SRC_FILES := castengine/main.cpp\
			$(CPP_DIR_ZZ:$(LOCAL_PATH)/%=%)\
			$(CPP_DIR_CE:$(LOCAL_PATH)/%=%)\
			$(CPP_DIR_CLASSES:$(LOCAL_PATH)/%=%)\
			../../Classes/ext/json/json_value.cpp\
			../../Classes/ext/json/json_reader.cpp\
			../../Classes/ext/json/json_writer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes\
			$(LOCAL_PATH)/../../Classes/ext/json\
			$(LOCAL_PATH)/../../Classes/ZZ\
			$(LOCAL_PATH)/../../Classes/CastEngine

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
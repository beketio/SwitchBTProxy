#cmake_minimum_required(VERSION 3.16.0)

set(srcs "src/bt_gap_api.cpp"
         "src/bt_hid_device_api.cpp"
         "src/bt_hid_host_api.cpp"
         "src/bt_hid_input_device.cpp"
         "src/bt_utils.cpp")

set(include_dirs "include" 
                "${IDF_PATH}/components/bt/common/include"
                "${IDF_PATH}/components/bt/common/btc/include"
                "${IDF_PATH}/components/bt/common/osi/include"
                "${IDF_PATH}/components/bt/host/bluedroid/common/include"
                "${IDF_PATH}/components/bt/host/bluedroid/bta/include"
                "${IDF_PATH}/components/bt/host/bluedroid/stack/include")

set(priv_include_dirs "private")

#list(APPEND include_dirs 
#        "$(IDF_PATH)/components/bt/host/bluedroid/common/include"
#        "$(IDF_PATH)/components/bt/host/bluedroid/bta/include/bta")

#if(CONFIG_BT_ENABLED)
#    if(CONFIG_BT_BLUEDROID_ENABLED)
#        list(APPEND srcs
#             "src/ble_hidd.c"
#             "src/ble_hidh.c"
#             "src/bt_hidh.c")
#    endif()
#endif()

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       PRIV_INCLUDE_DIRS "${priv_include_dirs}"
                       REQUIRES esp_event bt esp_hid)


#include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#project(SwitchProxy)
set(srcs "src/bta_hd/bta_hd_act.cc"
        "src/bta_hd/bta_hd_api.cc"
        "src/bta_hd/bta_hd_main.cc"
        "src/stack/hidd_api.cc"
        "src/stack/hidd_conn.cc")

set(include_dirs "include")

set(priv_include_dirs "private")

list(APPEND priv_include_dirs 
                "${IDF_PATH}/components/bt/common/include"
                "${IDF_PATH}/components/bt/common/btc/include"
                "${IDF_PATH}/components/bt/common/osi/include"
                "${IDF_PATH}/components/bt/host/bluedroid/common/include"
                "${IDF_PATH}/components/bt/host/bluedroid/bta/include"
                "${IDF_PATH}/components/bt/host/bluedroid/stack/include")

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
                       REQUIRES esp_event bt)
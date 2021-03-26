#ifndef CUSTOM_BT_TARGET_H
#define CUSTOM_BT_TARGET_H

#define BTA_HD_INCLUDED true

// Should be in bt_target.h
#ifndef HID_DEV_MTU_SIZE
    #define HID_DEV_MTU_SIZE BT_DEFAULT_BUFFER_SIZE // 64 in fork
#endif

#ifndef HID_DEV_FLUSH_TO
    #define HID_DEV_FLUSH_TO 0xffff
#endif

#ifndef BTM_SEC_SERVICE_HIDD_SEC_CTRL
    #define BTM_SEC_SERVICE_HIDD_SEC_CTRL       51
#endif
#ifndef BTM_SEC_SERVICE_HIDD_NOSEC_CTRL
    #define BTM_SEC_SERVICE_HIDD_NOSEC_CTRL     52
#endif
#ifndef BTM_SEC_SERVICE_HIDD_INTR
    #define BTM_SEC_SERVICE_HIDD_INTR           53
#endif

#endif // CUSTOM_BT_TARGET_H
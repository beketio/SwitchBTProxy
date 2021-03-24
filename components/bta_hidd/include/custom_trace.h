#pragma once

#include "common/bt_trace.h"

#ifdef CONFIG_HIDD_INITIAL_TRACE_LEVEL
#define HIDD_INITIAL_TRACE_LEVEL            CONFIG_HIDD_INITIAL_TRACE_LEVEL
#else
#define HIDD_INITIAL_TRACE_LEVEL             BT_TRACE_LEVEL_WARNING
#endif

/* define traces for HID Device */
#define HIDD_TRACE_ERROR(fmt, args...)      {if (hd_cb.trace_level >= BT_TRACE_LEVEL_ERROR && BT_LOG_LEVEL_CHECK(HIDD, ERROR)) BT_PRINT_E("BT_HIDD", fmt, ## args);}
#define HIDD_TRACE_WARNING(fmt, args...)    {if (hd_cb.trace_level >= BT_TRACE_LEVEL_WARNING && BT_LOG_LEVEL_CHECK(HIDD, WARNING)) BT_PRINT_W("BT_HIDD", fmt, ## args);}
#define HIDD_TRACE_API(fmt, args...)        {if (hd_cb.trace_level >= BT_TRACE_LEVEL_API && BT_LOG_LEVEL_CHECK(HIDD,API)) BT_PRINT_I("BT_HIDD", fmt, ## args);}
#define HIDD_TRACE_EVENT(fmt, args...)      {if (hd_cb.trace_level >= BT_TRACE_LEVEL_EVENT && BT_LOG_LEVEL_CHECK(HIDD,EVENT)) BT_PRINT_D("BT_HIDD", fmt, ## args);}
#define HIDD_TRACE_DEBUG(fmt, args...)      {if (hd_cb.trace_level >= BT_TRACE_LEVEL_DEBUG && BT_LOG_LEVEL_CHECK(HIDD,DEBUG)) BT_PRINT_D("BT_HIDD", fmt, ## args);}
#define HIDD_TRACE_VERBOSE(fmt, args...)    {if (hd_cb.trace_level >= BT_TRACE_LEVEL_VERBOSE && BT_LOG_LEVEL_CHECK(HIDD,VERBOSE)) BT_PRINT_D("BT_HIDD", fmt, ## args);}
#include <lcm_drv.h>
//superdragonpt added
extern LCM_DRIVER lgld070wx3_dsi_vdo_lcm_drv;
extern LCM_DRIVER nt35521_dsi_vdo_lcm_drv;
//superdragonpt

LCM_DRIVER* lcm_driver_list[] = 
{ 
//superdragonpt added
#if defined(LGLD070WX3_DSI_VDO)
    &lgld070wx3_dsi_vdo_lcm_drv,
#endif

#if defined(NT35521_DSI_VDO)
        &nt35521_dsi_vdo_lcm_drv,
#endif
//superdragonpt
};

#define LCM_COMPILE_ASSERT(condition) LCM_COMPILE_ASSERT_X(condition, __LINE__)
#define LCM_COMPILE_ASSERT_X(condition, line) LCM_COMPILE_ASSERT_XX(condition, line)
#define LCM_COMPILE_ASSERT_XX(condition, line) char assertion_failed_at_line_##line[(condition)?1:-1]

unsigned int lcm_count = sizeof(lcm_driver_list)/sizeof(LCM_DRIVER*);

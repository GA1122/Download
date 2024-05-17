ar6000_init_module(void)
{
    static int probed = 0;
    int r;
    OSDRV_CALLBACKS osdrvCallbacks;

    a_module_debug_support_init();

#ifdef DEBUG
         
    if (debughtc != 0) {
        ATH_DEBUG_SET_DEBUG_MASK(htc,debughtc);
    }
    if (debugbmi != 0) {
        ATH_DEBUG_SET_DEBUG_MASK(bmi,debugbmi);
    }
    if (debughif != 0) {
        ATH_DEBUG_SET_DEBUG_MASK(hif,debughif);
    }
    if (debugdriver != 0) {
        ATH_DEBUG_SET_DEBUG_MASK(driver,debugdriver);
    }

#endif

    A_REGISTER_MODULE_DEBUG_INFO(driver);

    A_MEMZERO(&osdrvCallbacks,sizeof(osdrvCallbacks));
    osdrvCallbacks.deviceInsertedHandler = ar6000_avail_ev;
    osdrvCallbacks.deviceRemovedHandler = ar6000_unavail_ev;
#ifdef CONFIG_PM
    osdrvCallbacks.deviceSuspendHandler = ar6000_suspend_ev;
    osdrvCallbacks.deviceResumeHandler = ar6000_resume_ev;
    osdrvCallbacks.devicePowerChangeHandler = ar6000_power_change_ev;
#endif

#ifdef DEBUG
     
    if(debugflags != 0)
    {
        g_dbg_flags = debugflags;
    }
#endif

    if (probed) {
        return -ENODEV;
    }
    probed++;

#ifdef ADAPTIVE_POWER_THROUGHPUT_CONTROL
    memset(&aptcTR, 0, sizeof(APTC_TRAFFIC_RECORD));
#endif  

    r = HIFInit(&osdrvCallbacks);
    if (r)
        return r;

    return 0;
}
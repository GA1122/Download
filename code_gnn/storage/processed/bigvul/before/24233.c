static void ar6000_target_failure(void *Instance, int Status)
{
    struct ar6_softc *ar = (struct ar6_softc *)Instance;
    WMI_TARGET_ERROR_REPORT_EVENT errEvent;
    static bool sip = false;

    if (Status != 0) {

        printk(KERN_ERR "ar6000_target_failure: target asserted \n");

        if (timer_pending(&ar->arHBChallengeResp.timer)) {
            A_UNTIMEOUT(&ar->arHBChallengeResp.timer);
        }

         
        ar6000_dump_target_assert_info(ar->arHifDevice,ar->arTargetType);

         
        ar6000_dbglog_get_debug_logs(ar);

         
        if (!sip) {
            sip = true;
            errEvent.errorVal = WMI_TARGET_COM_ERR |
                                WMI_TARGET_FATAL_ERR;
        }
    }
}

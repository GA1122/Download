ScreenSaverFreeSuspend(void *value, XID id)
{
    ScreenSaverSuspensionPtr data = (ScreenSaverSuspensionPtr) value;
    ScreenSaverSuspensionPtr *prev, this;

     
    for (prev = &suspendingClients; (this = *prev); prev = &this->next) {
        if (this == data) {
            *prev = this->next;
            free(this);
            break;
        }
    }

     
    if (screenSaverSuspended && suspendingClients == NULL) {
        screenSaverSuspended = FALSE;

         
#ifdef DPMSExtension
        if (screenIsSaved != SCREEN_SAVER_ON && DPMSPowerLevel == DPMSModeOn)
#else
        if (screenIsSaved != SCREEN_SAVER_ON)
#endif
        {
            DeviceIntPtr dev;
            UpdateCurrentTimeIf();
            nt_list_for_each_entry(dev, inputInfo.devices, next)
                NoticeTime(dev, currentTime);
            SetScreenSaverTimer();
        }
    }

    return Success;
}

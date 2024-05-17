ScreenSaverExtensionInit(void)
{
    ExtensionEntry *extEntry;
    int i;
    ScreenPtr pScreen;

    if (!dixRegisterPrivateKey(&ScreenPrivateKeyRec, PRIVATE_SCREEN, 0))
        return;

    AttrType = CreateNewResourceType(ScreenSaverFreeAttr, "SaverAttr");
    SaverEventType = CreateNewResourceType(ScreenSaverFreeEvents, "SaverEvent");
    SuspendType = CreateNewResourceType(ScreenSaverFreeSuspend, "SaverSuspend");

    for (i = 0; i < screenInfo.numScreens; i++) {
        pScreen = screenInfo.screens[i];
        SetScreenPrivate(pScreen, NULL);
    }
    if (AttrType && SaverEventType && SuspendType &&
        (extEntry = AddExtension(ScreenSaverName, ScreenSaverNumberEvents, 0,
                                 ProcScreenSaverDispatch,
                                 SProcScreenSaverDispatch, NULL,
                                 StandardMinorOpcode))) {
        ScreenSaverEventBase = extEntry->eventBase;
        EventSwapVector[ScreenSaverEventBase] =
            (EventSwapPtr) SScreenSaverNotifyEvent;
    }
}

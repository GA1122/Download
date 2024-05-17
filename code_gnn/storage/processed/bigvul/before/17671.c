XFixesCursorInit(void)
{
    int i;

    if (party_like_its_1989)
        CursorVisible = EnableCursor;
    else
        CursorVisible = FALSE;

    if (!dixRegisterPrivateKey(&CursorScreenPrivateKeyRec, PRIVATE_SCREEN, 0))
        return FALSE;

    for (i = 0; i < screenInfo.numScreens; i++) {
        ScreenPtr pScreen = screenInfo.screens[i];
        CursorScreenPtr cs;

        cs = (CursorScreenPtr) calloc(1, sizeof(CursorScreenRec));
        if (!cs)
            return FALSE;
        Wrap(cs, pScreen, CloseScreen, CursorCloseScreen);
        Wrap(cs, pScreen, DisplayCursor, CursorDisplayCursor);
        cs->pCursorHideCounts = NULL;
        SetCursorScreen(pScreen, cs);
    }
    CursorClientType = CreateNewResourceType(CursorFreeClient,
                                             "XFixesCursorClient");
    CursorHideCountType = CreateNewResourceType(CursorFreeHideCount,
                                                "XFixesCursorHideCount");
    CursorWindowType = CreateNewResourceType(CursorFreeWindow,
                                             "XFixesCursorWindow");

    return CursorClientType && CursorHideCountType && CursorWindowType;
}

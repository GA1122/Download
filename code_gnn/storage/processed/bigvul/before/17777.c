AddGPUScreen(Bool (*pfnInit) (ScreenPtr   ,
                              int   ,
                              char **       
                              ),
             int argc, char **argv)
{
    int i;
    ScreenPtr pScreen;
    Bool ret;

    i = screenInfo.numGPUScreens;
    if (i == MAXGPUSCREENS)
        return -1;

    pScreen = (ScreenPtr) calloc(1, sizeof(ScreenRec));
    if (!pScreen)
        return -1;

    ret = init_screen(pScreen, i, TRUE);
    if (ret != 0) {
        free(pScreen);
        return ret;
    }

     
    screenInfo.gpuscreens[i] = pScreen;
    screenInfo.numGPUScreens++;
    if (!(*pfnInit) (pScreen, argc, argv)) {
        dixFreePrivates(pScreen->devPrivates, PRIVATE_SCREEN);
        free(pScreen);
        screenInfo.numGPUScreens--;
        return -1;
    }

    update_desktop_dimensions();

     
    if (!dixPrivatesCreated(PRIVATE_CURSOR))
        dixRegisterScreenPrivateKey(&cursorScreenDevPriv, pScreen,
                                    PRIVATE_CURSOR, 0);

    return i;
}

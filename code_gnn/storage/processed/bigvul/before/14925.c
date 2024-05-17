AddScreen(Bool (*pfnInit) (ScreenPtr   ,
                           int   ,
                           char **       
          ), int argc, char **argv)
{

    int i;
    ScreenPtr pScreen;
    Bool ret;

    i = screenInfo.numScreens;
    if (i == MAXSCREENS)
        return -1;

    pScreen = (ScreenPtr) calloc(1, sizeof(ScreenRec));
    if (!pScreen)
        return -1;

    ret = init_screen(pScreen, i, FALSE);
    if (ret != 0) {
        free(pScreen);
        return ret;
    }
     
    screenInfo.screens[i] = pScreen;
    screenInfo.numScreens++;
    if (!(*pfnInit) (pScreen, argc, argv)) {
        dixFreeScreenSpecificPrivates(pScreen);
        dixFreePrivates(pScreen->devPrivates, PRIVATE_SCREEN);
        free(pScreen);
        screenInfo.numScreens--;
        return -1;
    }

    update_desktop_dimensions();

    dixRegisterScreenPrivateKey(&cursorScreenDevPriv, pScreen, PRIVATE_CURSOR,
                                0);

    return i;
}

DGAVTSwitch(void)
{
    ScreenPtr pScreen;
    int i;

    for (i = 0; i < screenInfo.numScreens; i++) {
        pScreen = screenInfo.screens[i];

         

        if (DGAScreenKeyRegistered) {
            DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

            if (pScreenPriv && pScreenPriv->current)
                return FALSE;
        }
    }

    return TRUE;
}

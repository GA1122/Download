DGACloseScreen(ScreenPtr pScreen)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

    if (mieq_installed) {
        mieqSetHandler(ET_DGAEvent, NULL);
        mieq_installed = FALSE;
    }

    FreeMarkedVisuals(pScreen);

    pScreen->CloseScreen = pScreenPriv->CloseScreen;
    pScreen->DestroyColormap = pScreenPriv->DestroyColormap;
    pScreen->InstallColormap = pScreenPriv->InstallColormap;
    pScreen->UninstallColormap = pScreenPriv->UninstallColormap;

     

    free(pScreenPriv);

    return ((*pScreen->CloseScreen) (pScreen));
}

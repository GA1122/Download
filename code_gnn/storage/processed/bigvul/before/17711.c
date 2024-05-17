DGAInit(ScreenPtr pScreen, DGAFunctionPtr funcs, DGAModePtr modes, int num)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    DGAScreenPtr pScreenPriv;
    int i;

    if (!funcs || !funcs->SetMode || !funcs->OpenFramebuffer)
        return FALSE;

    if (!modes || num <= 0)
        return FALSE;

    if (!dixRegisterPrivateKey(&DGAScreenKeyRec, PRIVATE_SCREEN, 0))
        return FALSE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

    if (!pScreenPriv) {
        if (!(pScreenPriv = (DGAScreenPtr) malloc(sizeof(DGAScreenRec))))
            return FALSE;
        dixSetPrivate(&pScreen->devPrivates, &DGAScreenKeyRec, pScreenPriv);
        pScreenPriv->CloseScreen = pScreen->CloseScreen;
        pScreen->CloseScreen = DGACloseScreen;
        pScreenPriv->DestroyColormap = pScreen->DestroyColormap;
        pScreen->DestroyColormap = DGADestroyColormap;
        pScreenPriv->InstallColormap = pScreen->InstallColormap;
        pScreen->InstallColormap = DGAInstallColormap;
        pScreenPriv->UninstallColormap = pScreen->UninstallColormap;
        pScreen->UninstallColormap = DGAUninstallColormap;
    }

    pScreenPriv->pScrn = pScrn;
    pScreenPriv->numModes = num;
    pScreenPriv->modes = modes;
    pScreenPriv->current = NULL;

    pScreenPriv->funcs = funcs;
    pScreenPriv->input = 0;
    pScreenPriv->client = NULL;
    pScreenPriv->fakedVisuals = NULL;
    pScreenPriv->dgaColormap = NULL;
    pScreenPriv->savedColormap = NULL;
    pScreenPriv->grabMouse = FALSE;
    pScreenPriv->grabKeyboard = FALSE;

    for (i = 0; i < num; i++)
        modes[i].num = i + 1;

#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        for (i = 0; i < num; i++)
            modes[i].flags &= ~DGA_PIXMAP_AVAILABLE;
#endif

    return TRUE;
}

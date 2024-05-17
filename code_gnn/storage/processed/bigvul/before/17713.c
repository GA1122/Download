DGAInstallColormap(ColormapPtr pmap)
{
    ScreenPtr pScreen = pmap->pScreen;
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

    if (pScreenPriv->current && pScreenPriv->dgaColormap) {
        if (pmap != pScreenPriv->dgaColormap) {
            pScreenPriv->savedColormap = pmap;
            pmap = pScreenPriv->dgaColormap;
        }
    }

    pScreen->InstallColormap = pScreenPriv->InstallColormap;
    (*pScreen->InstallColormap) (pmap);
    pScreen->InstallColormap = DGAInstallColormap;
}

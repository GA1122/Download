DGAInstallCmap(ColormapPtr cmap)
{
    ScreenPtr pScreen = cmap->pScreen;
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

     

    if (!pScreenPriv->dgaColormap)
        pScreenPriv->savedColormap = GetInstalledmiColormap(pScreen);

    pScreenPriv->dgaColormap = cmap;

    (*pScreen->InstallColormap) (cmap);
}

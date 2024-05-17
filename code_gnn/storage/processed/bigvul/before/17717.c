DGAReInitModes(ScreenPtr pScreen, DGAModePtr modes, int num)
{
    DGAScreenPtr pScreenPriv;
    int i;

     
    if (!DGAScreenKeyRegistered)
        return TRUE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

     
    if (!pScreenPriv)
        return TRUE;

     
    if (pScreenPriv->current)
        return FALSE;

     
    if (!num)
        modes = NULL;
    else if (!modes)
        num = 0;

    pScreenPriv->numModes = num;
    pScreenPriv->modes = modes;

     
    if (!num)
        return TRUE;

    for (i = 0; i < num; i++)
        modes[i].num = i + 1;

#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        for (i = 0; i < num; i++)
            modes[i].flags &= ~DGA_PIXMAP_AVAILABLE;
#endif

    return TRUE;
}

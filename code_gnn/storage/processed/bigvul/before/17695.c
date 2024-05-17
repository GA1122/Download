DGAActive(int index)
{
    DGAScreenPtr pScreenPriv;

    if (!DGAScreenKeyRegistered)
        return FALSE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

    if (pScreenPriv && pScreenPriv->current)
        return TRUE;

    return FALSE;
}

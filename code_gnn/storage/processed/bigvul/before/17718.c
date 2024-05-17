DGAScreenAvailable(ScreenPtr pScreen)
{
    if (!DGAScreenKeyRegistered)
        return FALSE;

    if (DGA_GET_SCREEN_PRIV(pScreen))
        return TRUE;
    return FALSE;
}

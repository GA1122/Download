DGAGetModes(int index)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    return pScreenPriv->numModes;
}

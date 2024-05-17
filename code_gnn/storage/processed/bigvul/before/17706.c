DGAGetModeInfo(int index, XDGAModePtr mode, int num)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if ((num <= 0) || (num > pScreenPriv->numModes))
        return BadValue;

    DGACopyModeInfo(&(pScreenPriv->modes[num - 1]), mode);

    return Success;
}

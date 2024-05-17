DGASetMode(int index, int num, XDGAModePtr mode, PixmapPtr *pPix)
{
    ScrnInfoPtr pScrn = xf86Screens[index];
    DGADeviceRec device;
    int ret;

     

    ret = (*pScrn->SetDGAMode) (pScrn, num, &device);
    if ((ret == Success) && num) {
        DGACopyModeInfo(device.mode, mode);
        *pPix = device.pPix;
    }

    return ret;
}

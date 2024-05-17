DGASync(int index)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if (pScreenPriv->funcs->Sync)
        (*pScreenPriv->funcs->Sync) (pScreenPriv->pScrn);

    return Success;
}

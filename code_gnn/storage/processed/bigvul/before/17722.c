DGASetViewport(int index, int x, int y, int mode)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

    if (pScreenPriv->funcs->SetViewport)
        (*pScreenPriv->funcs->SetViewport) (pScreenPriv->pScrn, x, y, mode);
    return Success;
}

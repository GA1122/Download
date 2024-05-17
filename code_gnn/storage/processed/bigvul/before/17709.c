DGAGetViewportStatus(int index)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if (!pScreenPriv->funcs->GetViewport)
        return 0;

    return (*pScreenPriv->funcs->GetViewport) (pScreenPriv->pScrn);
}

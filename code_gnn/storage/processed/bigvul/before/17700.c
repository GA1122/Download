DGACloseFramebuffer(int index)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     
    if (pScreenPriv->funcs->CloseFramebuffer)
        (*pScreenPriv->funcs->CloseFramebuffer) (pScreenPriv->pScrn);
}

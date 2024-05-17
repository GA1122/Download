DGABlitTransRect(int index,
                 int srcx, int srcy,
                 int w, int h, int dstx, int dsty, unsigned long color)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if (pScreenPriv->funcs->BlitTransRect &&
        (pScreenPriv->current->mode->flags & DGA_BLIT_RECT_TRANS)) {

        (*pScreenPriv->funcs->BlitTransRect) (pScreenPriv->pScrn,
                                              srcx, srcy, w, h, dstx, dsty,
                                              color);
        return Success;
    }
    return BadMatch;
}

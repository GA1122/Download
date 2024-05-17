DGABlitRect(int index, int srcx, int srcy, int w, int h, int dstx, int dsty)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if (pScreenPriv->funcs->BlitRect &&
        (pScreenPriv->current->mode->flags & DGA_BLIT_RECT)) {

        (*pScreenPriv->funcs->BlitRect) (pScreenPriv->pScrn,
                                         srcx, srcy, w, h, dstx, dsty);
        return Success;
    }
    return BadMatch;
}

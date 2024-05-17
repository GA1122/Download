DGAFillRect(int index, int x, int y, int w, int h, unsigned long color)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    if (pScreenPriv->funcs->FillRect &&
        (pScreenPriv->current->mode->flags & DGA_FILL_RECT)) {

        (*pScreenPriv->funcs->FillRect) (pScreenPriv->pScrn, x, y, w, h, color);
        return Success;
    }
    return BadMatch;
}

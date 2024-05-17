DGAOpenFramebuffer(int index,
                   char **name,
                   unsigned char **mem, int *size, int *offset, int *flags)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     

    return (*pScreenPriv->funcs->OpenFramebuffer) (pScreenPriv->pScrn,
                                                   name, mem, size, offset,
                                                   flags);
}

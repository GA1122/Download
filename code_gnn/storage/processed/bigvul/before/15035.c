dixDestroyPixmap(void *value, XID pid)
{
    PixmapPtr pPixmap = (PixmapPtr) value;

    return (*pPixmap->drawable.pScreen->DestroyPixmap) (pPixmap);
}

FreeAttrs(ScreenSaverAttrPtr pAttr)
{
    PixmapPtr pPixmap;
    CursorPtr pCursor;

    if ((pPixmap = pAttr->pBackgroundPixmap) != 0)
        (*pPixmap->drawable.pScreen->DestroyPixmap) (pPixmap);
    if ((pPixmap = pAttr->pBorderPixmap) != 0)
        (*pPixmap->drawable.pScreen->DestroyPixmap) (pPixmap);
    if ((pCursor = pAttr->pCursor) != 0)
        FreeCursor(pCursor, (Cursor) 0);
}

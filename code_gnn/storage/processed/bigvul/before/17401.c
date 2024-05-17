FreeScreenAttr(ScreenSaverAttrPtr pAttr)
{
    FreeAttrs(pAttr);
    free(pAttr->values);
    free(pAttr);
}

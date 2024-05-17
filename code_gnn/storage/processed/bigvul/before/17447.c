ResFindResourcePixmaps(void *value, XID id, RESTYPE type, void *cdata)
{
    SizeType sizeFunc = GetResourceTypeSizeFunc(type);
    ResourceSizeRec size = { 0, 0, 0 };
    unsigned long *bytes = cdata;

    sizeFunc(value, id, &size);
    *bytes += size.pixmapRefSize;
}

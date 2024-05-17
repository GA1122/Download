XRecordIdBaseMask(Display *dpy)
{
    return 0x1fffffff & ~dpy->resource_mask;
}

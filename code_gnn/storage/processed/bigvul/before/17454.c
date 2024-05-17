SwapXResResourceSizeSpec(xXResResourceSizeSpec *size)
{
    SwapXResResourceIdSpec(&size->spec);
    swapl(&size->bytes);
    swapl(&size->refCount);
    swapl(&size->useCount);
}

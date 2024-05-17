SwapXResResourceIdSpec(xXResResourceIdSpec *spec)
{
    swapl(&spec->resource);
    swapl(&spec->type);
}

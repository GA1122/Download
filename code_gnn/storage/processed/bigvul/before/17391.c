XineramaFindIDByScrnum(void *resource, XID id, void *privdata)
{
    PanoramiXRes *res = (PanoramiXRes *) resource;
    PanoramiXSearchData *data = (PanoramiXSearchData *) privdata;

    return res->info[data->screen].id == data->id;
}

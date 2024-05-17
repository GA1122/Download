PanoramiXFindIDByScrnum(RESTYPE type, XID id, int screen)
{
    PanoramiXSearchData data;
    void *val;

    if (!screen) {
        dixLookupResourceByType(&val, id, type, serverClient, DixReadAccess);
        return val;
    }

    data.screen = screen;
    data.id = id;

    return LookupClientResourceComplex(clients[CLIENT_ID(id)], type,
                                       XineramaFindIDByScrnum, &data);
}

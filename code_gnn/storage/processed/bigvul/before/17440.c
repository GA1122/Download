ProcResDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
    case X_XResQueryVersion:
        return ProcXResQueryVersion(client);
    case X_XResQueryClients:
        return ProcXResQueryClients(client);
    case X_XResQueryClientResources:
        return ProcXResQueryClientResources(client);
    case X_XResQueryClientPixmapBytes:
        return ProcXResQueryClientPixmapBytes(client);
    case X_XResQueryClientIds:
        return ProcXResQueryClientIds(client);
    case X_XResQueryResourceBytes:
        return ProcXResQueryResourceBytes(client);
    default: break;
    }

    return BadRequest;
}

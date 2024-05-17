SProcXResQueryClientResources(ClientPtr client)
{
    REQUEST(xXResQueryClientResourcesReq);
    REQUEST_SIZE_MATCH(xXResQueryClientResourcesReq);
    swapl(&stuff->xid);
    return ProcXResQueryClientResources(client);
}

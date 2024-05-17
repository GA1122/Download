SProcResourceReq(ClientPtr client)
{
    REQUEST(xResourceReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xResourceReq);         
    swapl(&stuff->id);
    return (*ProcVector[stuff->reqType]) (client);
}

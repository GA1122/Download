SProcXvQueryExtension(ClientPtr client)
{
    REQUEST(xvQueryExtensionReq);
    REQUEST_SIZE_MATCH(xvQueryExtensionReq);
    swaps(&stuff->length);
    return XvProcVector[xv_QueryExtension] (client);
}

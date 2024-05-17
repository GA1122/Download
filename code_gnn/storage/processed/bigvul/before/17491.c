SProcXvQueryEncodings(ClientPtr client)
{
    REQUEST(xvQueryEncodingsReq);
    REQUEST_SIZE_MATCH(xvQueryEncodingsReq);
    swaps(&stuff->length);
    swapl(&stuff->port);
    return XvProcVector[xv_QueryEncodings] (client);
}

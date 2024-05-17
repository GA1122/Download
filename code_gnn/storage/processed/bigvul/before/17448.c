SProcXResQueryClientIds (ClientPtr client)
{
    REQUEST(xXResQueryClientIdsReq);

    REQUEST_AT_LEAST_SIZE (xXResQueryClientIdsReq);
    swapl(&stuff->numSpecs);
    return ProcXResQueryClientIds(client);
}

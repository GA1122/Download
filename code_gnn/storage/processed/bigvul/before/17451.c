SProcXResQueryVersion(ClientPtr client)
{
    REQUEST_SIZE_MATCH(xXResQueryVersionReq);
    return ProcXResQueryVersion(client);
}

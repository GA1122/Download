ProcXFixesDispatch(ClientPtr client)
{
    REQUEST(xXFixesReq);
    XFixesClientPtr pXFixesClient = GetXFixesClient(client);

    if (pXFixesClient->major_version >= NUM_VERSION_REQUESTS)
        return BadRequest;
    if (stuff->xfixesReqType > version_requests[pXFixesClient->major_version])
        return BadRequest;
    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}

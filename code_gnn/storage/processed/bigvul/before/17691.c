SProcXFixesDispatch(ClientPtr client)
{
    REQUEST(xXFixesReq);
    if (stuff->xfixesReqType >= XFixesNumberRequests)
        return BadRequest;
    return (*SProcXFixesVector[stuff->xfixesReqType]) (client);
}

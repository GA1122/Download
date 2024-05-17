ProcXvDispatch(ClientPtr client)
{
    REQUEST(xReq);

    UpdateCurrentTime();

    if (stuff->data >= xvNumRequests) {
        return BadRequest;
    }

    return XvProcVector[stuff->data] (client);
}

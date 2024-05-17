ProcScreenSaverDispatch(ClientPtr client)
{
    REQUEST(xReq);

    if (stuff->data < NUM_REQUESTS)
        return (*NormalVector[stuff->data]) (client);
    return BadRequest;
}

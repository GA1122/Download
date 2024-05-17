SProcScreenSaverDispatch(ClientPtr client)
{
    REQUEST(xReq);

    if (stuff->data < NUM_REQUESTS)
        return (*SwappedVector[stuff->data]) (client);
    return BadRequest;
}

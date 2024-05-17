SProcRenderCreateLinearGradient(ClientPtr client)
{
    int len;

    REQUEST(xRenderCreateLinearGradientReq);
    REQUEST_AT_LEAST_SIZE(xRenderCreateLinearGradientReq);

    swaps(&stuff->length);
    swapl(&stuff->pid);
    swapl(&stuff->p1.x);
    swapl(&stuff->p1.y);
    swapl(&stuff->p2.x);
    swapl(&stuff->p2.y);
    swapl(&stuff->nStops);

    len = (client->req_len << 2) - sizeof(xRenderCreateLinearGradientReq);
    if (stuff->nStops > UINT32_MAX / (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;
    if (len != stuff->nStops * (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff + 1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

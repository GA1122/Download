SProcRenderCreateConicalGradient(ClientPtr client)
{
    int len;

    REQUEST(xRenderCreateConicalGradientReq);
    REQUEST_AT_LEAST_SIZE(xRenderCreateConicalGradientReq);

    swaps(&stuff->length);
    swapl(&stuff->pid);
    swapl(&stuff->center.x);
    swapl(&stuff->center.y);
    swapl(&stuff->angle);
    swapl(&stuff->nStops);

    len = (client->req_len << 2) - sizeof(xRenderCreateConicalGradientReq);
    if (stuff->nStops > UINT32_MAX / (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;
    if (len != stuff->nStops * (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff + 1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

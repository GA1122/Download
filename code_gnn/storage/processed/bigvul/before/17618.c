SProcRenderCreateRadialGradient(ClientPtr client)
{
    int len;

    REQUEST(xRenderCreateRadialGradientReq);
    REQUEST_AT_LEAST_SIZE(xRenderCreateRadialGradientReq);

    swaps(&stuff->length);
    swapl(&stuff->pid);
    swapl(&stuff->inner.x);
    swapl(&stuff->inner.y);
    swapl(&stuff->outer.x);
    swapl(&stuff->outer.y);
    swapl(&stuff->inner_radius);
    swapl(&stuff->outer_radius);
    swapl(&stuff->nStops);

    len = (client->req_len << 2) - sizeof(xRenderCreateRadialGradientReq);
    if (stuff->nStops > UINT32_MAX / (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;
    if (len != stuff->nStops * (sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff + 1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

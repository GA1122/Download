SProcRenderComposite(ClientPtr client)
{
    REQUEST(xRenderCompositeReq);
    REQUEST_SIZE_MATCH(xRenderCompositeReq);
    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->mask);
    swapl(&stuff->dst);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    swaps(&stuff->xMask);
    swaps(&stuff->yMask);
    swaps(&stuff->xDst);
    swaps(&stuff->yDst);
    swaps(&stuff->width);
    swaps(&stuff->height);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

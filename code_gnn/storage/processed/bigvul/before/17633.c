SProcRenderSetPictureTransform(ClientPtr client)
{
    REQUEST(xRenderSetPictureTransformReq);
    REQUEST_SIZE_MATCH(xRenderSetPictureTransformReq);

    swaps(&stuff->length);
    swapl(&stuff->picture);
    swapl(&stuff->transform.matrix11);
    swapl(&stuff->transform.matrix12);
    swapl(&stuff->transform.matrix13);
    swapl(&stuff->transform.matrix21);
    swapl(&stuff->transform.matrix22);
    swapl(&stuff->transform.matrix23);
    swapl(&stuff->transform.matrix31);
    swapl(&stuff->transform.matrix32);
    swapl(&stuff->transform.matrix33);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

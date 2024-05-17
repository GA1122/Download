SProcXFixesDestroyPointerBarrier(ClientPtr client)
{
    REQUEST(xXFixesDestroyPointerBarrierReq);

    swaps(&stuff->length);
    REQUEST_SIZE_MATCH(xXFixesDestroyPointerBarrierReq);
    swapl(&stuff->barrier);
    return ProcXFixesVector[stuff->xfixesReqType] (client);
}

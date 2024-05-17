ProcXFixesDestroyPointerBarrier(ClientPtr client)
{
    REQUEST(xXFixesDestroyPointerBarrierReq);

    REQUEST_SIZE_MATCH(xXFixesDestroyPointerBarrierReq);

    return XIDestroyPointerBarrier(client, stuff);
}

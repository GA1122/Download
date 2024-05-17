ProcXFixesCreatePointerBarrier(ClientPtr client)
{
    REQUEST(xXFixesCreatePointerBarrierReq);

    REQUEST_FIXED_SIZE(xXFixesCreatePointerBarrierReq, pad_to_int32(stuff->num_devices));
    LEGAL_NEW_RESOURCE(stuff->barrier, client);

    return XICreatePointerBarrier(client, stuff);
}

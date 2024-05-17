ProcXResQueryClientIds (ClientPtr client)
{
    REQUEST(xXResQueryClientIdsReq);

    xXResClientIdSpec        *specs = (void*) ((char*) stuff + sizeof(*stuff));
    int                       rc;
    ConstructClientIdCtx      ctx;

    InitConstructClientIdCtx(&ctx);

    REQUEST_AT_LEAST_SIZE(xXResQueryClientIdsReq);
    REQUEST_FIXED_SIZE(xXResQueryClientIdsReq,
                       stuff->numSpecs * sizeof(specs[0]));

    rc = ConstructClientIds(client, stuff->numSpecs, specs, &ctx);

    if (rc == Success) {
        xXResQueryClientIdsReply  rep = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = bytes_to_int32(ctx.resultBytes),
            .numIds = ctx.numIds
        };

        assert((ctx.resultBytes & 3) == 0);

        if (client->swapped) {
            swaps (&rep.sequenceNumber);
            swapl (&rep.length);
            swapl (&rep.numIds);
        }

        WriteToClient(client, sizeof(rep), &rep);
        WriteFragmentsToClient(client, &ctx.response);
    }

    DestroyConstructClientIdCtx(&ctx);

    return rc;
}

ProcRenderQueryPictIndexValues(ClientPtr client)
{
    PictFormatPtr pFormat;
    int rc, num;
    int rlength;
    int i;

    REQUEST(xRenderQueryPictIndexValuesReq);
    xRenderQueryPictIndexValuesReply *reply;
    xIndexValue *values;

    REQUEST_AT_LEAST_SIZE(xRenderQueryPictIndexValuesReq);

    rc = dixLookupResourceByType((void **) &pFormat, stuff->format,
                                 PictFormatType, client, DixReadAccess);
    if (rc != Success)
        return rc;

    if (pFormat->type != PictTypeIndexed) {
        client->errorValue = stuff->format;
        return BadMatch;
    }
    num = pFormat->index.nvalues;
    rlength = (sizeof(xRenderQueryPictIndexValuesReply) +
               num * sizeof(xIndexValue));
    reply = (xRenderQueryPictIndexValuesReply *) calloc(1, rlength);
    if (!reply)
        return BadAlloc;

    reply->type = X_Reply;
    reply->sequenceNumber = client->sequence;
    reply->length = bytes_to_int32(rlength - sizeof(xGenericReply));
    reply->numIndexValues = num;

    values = (xIndexValue *) (reply + 1);

    memcpy(reply + 1, pFormat->index.pValues, num * sizeof(xIndexValue));

    if (client->swapped) {
        for (i = 0; i < num; i++) {
            swapl(&values[i].pixel);
            swaps(&values[i].red);
            swaps(&values[i].green);
            swaps(&values[i].blue);
            swaps(&values[i].alpha);
        }
        swaps(&reply->sequenceNumber);
        swapl(&reply->length);
        swapl(&reply->numIndexValues);
    }

    WriteToClient(client, rlength, reply);
    free(reply);
    return Success;
}

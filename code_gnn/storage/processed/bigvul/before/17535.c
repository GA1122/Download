ProcPseudoramiXIsActive(ClientPtr client)
{
     
    xXineramaIsActiveReply rep;

    TRACE;

    REQUEST_SIZE_MATCH(xXineramaIsActiveReq);

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.state = !noPseudoramiXExtension;
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.state);
    }
    WriteToClient(client, sizeof(xXineramaIsActiveReply),&rep);
    return Success;
}

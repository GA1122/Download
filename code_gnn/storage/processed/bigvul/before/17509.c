SWriteQueryAdaptorsReply(ClientPtr client, xvQueryAdaptorsReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swaps(&rep->num_adaptors);

    WriteToClient(client, sz_xvQueryAdaptorsReply, rep);

    return Success;
}

SWriteQueryExtensionReply(ClientPtr client, xvQueryExtensionReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swaps(&rep->version);
    swaps(&rep->revision);

    WriteToClient(client, sz_xvQueryExtensionReply, rep);

    return Success;
}

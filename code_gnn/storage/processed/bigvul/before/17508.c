SWriteListImageFormatsReply(ClientPtr client, xvListImageFormatsReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swapl(&rep->num_formats);

    WriteToClient(client, sz_xvListImageFormatsReply, rep);

    return Success;
}

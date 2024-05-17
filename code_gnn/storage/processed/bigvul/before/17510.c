SWriteQueryBestSizeReply(ClientPtr client, xvQueryBestSizeReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swaps(&rep->actual_width);
    swaps(&rep->actual_height);

    WriteToClient(client, sz_xvQueryBestSizeReply, rep);

    return Success;
}

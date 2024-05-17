SWriteQueryImageAttributesReply(ClientPtr client,
                                xvQueryImageAttributesReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swapl(&rep->num_planes);
    swapl(&rep->data_size);
    swaps(&rep->width);
    swaps(&rep->height);

    WriteToClient(client, sz_xvQueryImageAttributesReply, rep);

    return Success;
}

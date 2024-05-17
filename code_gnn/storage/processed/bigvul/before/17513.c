SWriteQueryPortAttributesReply(ClientPtr client,
                               xvQueryPortAttributesReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swapl(&rep->num_attributes);
    swapl(&rep->text_size);

    WriteToClient(client, sz_xvQueryPortAttributesReply, rep);

    return Success;
}

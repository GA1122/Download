SWriteGetPortAttributeReply(ClientPtr client, xvGetPortAttributeReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swapl(&rep->value);

    WriteToClient(client, sz_xvGetPortAttributeReply, rep);

    return Success;
}

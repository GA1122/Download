SWriteGrabPortReply(ClientPtr client, xvGrabPortReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);

    WriteToClient(client, sz_xvGrabPortReply, rep);

    return Success;
}

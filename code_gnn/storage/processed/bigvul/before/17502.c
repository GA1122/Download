SWriteAttributeInfo(ClientPtr client, xvAttributeInfo * pAtt)
{
    swapl(&pAtt->flags);
    swapl(&pAtt->size);
    swapl(&pAtt->min);
    swapl(&pAtt->max);
    WriteToClient(client, sz_xvAttributeInfo, pAtt);

    return Success;
}

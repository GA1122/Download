ProcXFixesGetCursorName(ClientPtr client)
{
    CursorPtr pCursor;
    xXFixesGetCursorNameReply reply;

    REQUEST(xXFixesGetCursorNameReq);
    const char *str;
    int len;

    REQUEST_SIZE_MATCH(xXFixesGetCursorNameReq);
    VERIFY_CURSOR(pCursor, stuff->cursor, client, DixGetAttrAccess);
    if (pCursor->name)
        str = NameForAtom(pCursor->name);
    else
        str = "";
    len = strlen(str);

    reply = (xXFixesGetCursorNameReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(len),
        .atom = pCursor->name,
        .nbytes = len
    };
    if (client->swapped) {
        swaps(&reply.sequenceNumber);
        swapl(&reply.length);
        swapl(&reply.atom);
        swaps(&reply.nbytes);
    }
    WriteReplyToClient(client, sizeof(xXFixesGetCursorNameReply), &reply);
    WriteToClient(client, len, str);

    return Success;
}

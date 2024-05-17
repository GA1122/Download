ProcXFixesChangeCursor(ClientPtr client)
{
    CursorPtr pSource, pDestination;

    REQUEST(xXFixesChangeCursorReq);

    REQUEST_SIZE_MATCH(xXFixesChangeCursorReq);
    VERIFY_CURSOR(pSource, stuff->source, client,
                  DixReadAccess | DixGetAttrAccess);
    VERIFY_CURSOR(pDestination, stuff->destination, client,
                  DixWriteAccess | DixSetAttrAccess);

    ReplaceCursor(pSource, TestForCursor, (void *) pDestination);
    return Success;
}

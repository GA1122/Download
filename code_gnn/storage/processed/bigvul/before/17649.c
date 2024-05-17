ProcXFixesChangeCursorByName(ClientPtr client)
{
    CursorPtr pSource;
    Atom name;
    char *tchar;

    REQUEST(xXFixesChangeCursorByNameReq);

    REQUEST_FIXED_SIZE(xXFixesChangeCursorByNameReq, stuff->nbytes);
    VERIFY_CURSOR(pSource, stuff->source, client,
                  DixReadAccess | DixGetAttrAccess);
    tchar = (char *) &stuff[1];
    name = MakeAtom(tchar, stuff->nbytes, FALSE);
    if (name)
        ReplaceCursor(pSource, TestForCursorName, &name);
    return Success;
}

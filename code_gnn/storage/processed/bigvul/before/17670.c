TestForCursorName(CursorPtr pCursor, void *closure)
{
    Atom *pName = closure;

    return pCursor->name == *pName;
}

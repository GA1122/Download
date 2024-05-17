static int strokeStyleToWxPenStyle(int p)
{
    if (p == SolidStroke)
        return wxSOLID;
    if (p == DottedStroke)
        return wxDOT;
    if (p == DashedStroke)
        return wxLONG_DASH;
    if (p == NoStroke)
        return wxTRANSPARENT;
    
    return wxSOLID;
}

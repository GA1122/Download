const char* RenderBlock::renderName() const
{
    if (isBody())
        return "RenderBody";  

    if (isFloating())
        return "RenderBlock (floating)";
    if (isOutOfFlowPositioned())
        return "RenderBlock (positioned)";
    if (isAnonymousColumnsBlock())
        return "RenderBlock (anonymous multi-column)";
    if (isAnonymousColumnSpanBlock())
        return "RenderBlock (anonymous multi-column span)";
    if (isAnonymousBlock())
        return "RenderBlock (anonymous)";
    if (isPseudoElement())
        return "RenderBlock (generated)";
    if (isAnonymous())
        return "RenderBlock (generated)";
    if (isRelPositioned())
        return "RenderBlock (relative positioned)";
    if (isStickyPositioned())
        return "RenderBlock (sticky positioned)";
    return "RenderBlock";
}

void IRCView::insertMarkerLine()  
{
    if (lastBlockIsLine(BlockIsMarker))
        return;

    appendLine(IRCView::MarkerLine);
}

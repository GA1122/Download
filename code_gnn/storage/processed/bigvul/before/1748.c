void IRCView::clearLines()
{
    while (hasLines())
    {
        voidLineBlock(m_lastMarkerLine->m_block);
    };
}

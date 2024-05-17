bool IRCView::lastBlockIsLine(int select)
{
    Burr *b = dynamic_cast<Burr*>(document()->lastBlock().userData());

    int state = -1;

    if (b)
        state = b->m_format;

    if (select == -1)
        return (state == BlockIsRemember || state == BlockIsMarker);

    return state == select;
}

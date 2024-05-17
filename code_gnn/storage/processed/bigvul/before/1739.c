Burr* IRCView::appendLine(IRCView::ObjectFormats type)
{
    ScrollBarPin barpin(verticalScrollBar());
    SelectionPin selpin(this);

    QTextCursor cursor(document());
    cursor.movePosition(QTextCursor::End);

    if (cursor.block().length() > 1)  
        cursor.insertBlock();
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), getFormat(type));

    QTextBlock block = cursor.block();
    Burr *b = new Burr(this, m_lastMarkerLine, block, type == MarkerLine? BlockIsMarker : BlockIsRemember);
    block.setUserData(b);

    m_lastMarkerLine = b;

    cursor.setPosition(block.position());

    return b;
}

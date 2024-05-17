void IRCView::voidLineBlock(QTextBlock rem)
{
    QTextCursor c(rem);

    c.select(QTextCursor::BlockUnderCursor);
    c.removeSelectedText();
}

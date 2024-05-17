void IRCView::doRawAppend(const QString& newLine, bool rtl)
{
    SelectionPin selpin(this);  
    QString line(newLine);

    line.remove('\n');

    KTextBrowser::append(line);

    QTextCursor formatCursor(document()->lastBlock());
    QTextBlockFormat format = formatCursor.blockFormat();

    if (!QApplication::isLeftToRight())
        rtl = !rtl;

    format.setAlignment(rtl ? Qt::AlignRight : Qt::AlignLeft);
    formatCursor.setBlockFormat(format);
}

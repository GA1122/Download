QMimeData *IRCView::createMimeDataFromSelection() const
{
    const QTextDocumentFragment fragment(textCursor());
    return new IrcViewMimeData(fragment);
}

void IRCView::doAppend(const QString& newLine, bool rtl, bool self)
{
    if (m_rememberLineDirtyBit)
        appendRememberLine();

    if (!self && m_chatWin)
        m_chatWin->activateTabNotification(m_tabNotification);

    int scrollMax = Preferences::self()->scrollbackMax();
    if (scrollMax != 0)
    {
        bool atBottom = (verticalScrollBar()->value() == verticalScrollBar()->maximum());
        document()->setMaximumBlockCount(atBottom ? scrollMax : document()->maximumBlockCount() + 1);
    }

    doRawAppend(newLine, rtl);

    if (!m_autoTextToSend.isEmpty() && m_server)
    {
        QString sendText = m_server->parseWildcards(m_autoTextToSend,m_server->getNickname(), QString(), QString(), QString(), QString());
        m_autoTextToSend.clear();
        emit autoText(sendText);
    }
    else
    {
        m_autoTextToSend.clear();
    }

    if (!m_lastStatusText.isEmpty())
        emit clearStatusBarTempText();
}

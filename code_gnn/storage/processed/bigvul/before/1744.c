void IRCView::appendRememberLine()
{
    m_rememberLineDirtyBit = false;

    if (lastBlockIsLine(BlockIsRemember))
        return;

    if (m_rememberLine)
    {
        QTextBlock rem = m_rememberLine->m_block;
        voidLineBlock(rem);
        if (m_rememberLine != 0)
        {
            m_rememberLine = 0;
        }
    }

    m_rememberLine = appendLine(IRCView::RememberLine);
}

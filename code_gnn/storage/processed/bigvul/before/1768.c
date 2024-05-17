void IRCView::insertRememberLine()  
{
    m_rememberLineDirtyBit = true;  

    if (!Preferences::self()->automaticRememberLineOnlyOnTextChange())
    {
        appendRememberLine();
    }
}

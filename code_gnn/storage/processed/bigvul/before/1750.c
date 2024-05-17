QString IRCView::createNickLine(const QString& nick, const QString& defaultColor, bool encapsulateNick, bool privMsg)
{
    QString nickLine = "%2";
    QString nickColor;

    if (Preferences::self()->useColoredNicks())
    {
        if (m_server)
        {
            if (nick != m_server->getNickname())
                nickColor = Preferences::self()->nickColor(m_server->obtainNickInfo(nick)->getNickColor()).name();
            else
                nickColor =  Preferences::self()->nickColor(8).name();
        }
        else if (m_chatWin->getType() == ChatWindow::DccChat)
        {
            QString ownNick = static_cast<DCC::ChatContainer*>(m_chatWin)->ownNick();

            if (nick != ownNick)
                nickColor = Preferences::self()->nickColor(Konversation::colorForNick(ownNick)).name();
            else
                nickColor = Preferences::self()->nickColor(8).name();
        }
    }
    else
        nickColor = defaultColor;

    nickLine = QLatin1String("<font color=\"") + nickColor + QLatin1String("\">") + nickLine + QLatin1String("</font>");

    if (Preferences::self()->useClickableNicks())
        nickLine = "<a class=\"nick\" href=\"#" + nick + "\">" + nickLine + "</a>";

    if (privMsg)
        nickLine.prepend(QLatin1String("-&gt; "));

    if(encapsulateNick)
        nickLine = QLatin1String("&lt;") + nickLine + QLatin1String("&gt;");

    if(Preferences::self()->useBoldNicks())
        nickLine = QLatin1String("<b>") + nickLine + QLatin1String("</b>");

    return nickLine;
}

void IRCView::append(const QString& nick, const QString& message)
{
    QString channelColor = Preferences::self()->color(Preferences::ChannelMessage).name();

    m_tabNotification = Konversation::tnfNormal;

    QString nickLine = createNickLine(nick, channelColor);

    QChar::Direction dir;
    QString text(filter(message, channelColor, nick, true, true, false, &dir));
    QString line;
    bool rtl = (dir == QChar::DirR);

    if(rtl)
    {
        line = RLE;
        line += LRE;
        line += "<font color=\"" + channelColor + "\">" + nickLine +" %1" + PDF + RLM + " %3</font>";
    }
    else
    {
        if (!QApplication::isLeftToRight())
            line += LRE;

        line += "<font color=\"" + channelColor + "\">%1" + nickLine + " %3</font>";
    }

    line = line.arg(timeStamp(), nick, text);

    emit textToLog(QString("<%1>\t%2").arg(nick, message));

    doAppend(line, rtl);
}

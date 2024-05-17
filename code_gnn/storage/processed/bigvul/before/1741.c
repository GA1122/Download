void IRCView::appendQuery(const QString& nick, const QString& message, bool inChannel)
{
    QString queryColor=Preferences::self()->color(Preferences::QueryMessage).name();

    m_tabNotification = Konversation::tnfPrivate;

    QString nickLine = createNickLine(nick, queryColor, true, inChannel);

    QString line;
    QChar::Direction dir;
    QString text(filter(message, queryColor, nick, true, true, false, &dir));
    bool rtl = (dir == QChar::DirR);

    if(rtl)
    {
        line = RLE;
        line += LRE;
        line += "<font color=\"" + queryColor + "\">" + nickLine + " %1" + PDF + RLM + " %3</font>";
    }
    else
    {
        if (!QApplication::isLeftToRight())
            line += LRE;

        line += "<font color=\"" + queryColor + "\">%1 " + nickLine + " %3</font>";
    }

    line = line.arg(timeStamp(), nick, text);

    if (inChannel) {
        emit textToLog(QString("<-> %1>\t%2").arg(nick, message));
    } else {
        emit textToLog(QString("<%1>\t%2").arg(nick, message));
    }

    doAppend(line, rtl);
}

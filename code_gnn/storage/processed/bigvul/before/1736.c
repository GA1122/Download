void IRCView::appendBacklogMessage(const QString& firstColumn,const QString& rawMessage)
{
    QString time;
    QString message = rawMessage;
    QString nick = firstColumn;
    QString backlogColor = Preferences::self()->color(Preferences::BacklogMessage).name();
    m_tabNotification = Konversation::tnfNone;

    int eot = nick.lastIndexOf(' ');
    time = nick.left(eot);
    nick = nick.mid(eot+1);

    if(!nick.isEmpty() && !nick.startsWith('<') && !nick.startsWith('*'))
    {
        nick = '|' + nick + '|';
    }

    nick.replace('<',"&lt;");
    nick.replace('>',"&gt;");

    QString line;
    QChar::Direction dir;
    QString text(filter(message, backlogColor, NULL, false, false, false, &dir));
    bool rtl = (dir == QChar::DirR);

    if(rtl)
    {
        line = RLE;
        line += LRE;
        line += "<font color=\"" + backlogColor + "\">%2 %1" + PDF + " %3</font>";
    }
    else
    {
        if (!QApplication::isLeftToRight())
            line += LRE;

        line += "<font color=\"" + backlogColor + "\">%1 %2 %3</font>";
    }

    line = line.arg(time, nick, text);

    doAppend(line, rtl);
}

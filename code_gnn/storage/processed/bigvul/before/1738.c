void IRCView::appendCommandMessage(const QString& type,const QString& message, bool parseURL, bool self)
{
    QString commandColor = Preferences::self()->color(Preferences::CommandMessage).name();
    QString prefix="***";
    m_tabNotification = Konversation::tnfControl;

    if(type == i18nc("Message type", "Join"))
    {
        prefix="-->";
        parseURL=false;
    }
    else if(type == i18nc("Message type", "Part") || type == i18nc("Message type", "Quit"))
    {
        prefix="<--";
    }

    prefix=Qt::escape(prefix);

    QString line;
    QChar::Direction dir;
    QString text(filter(message, commandColor, 0, true, parseURL, self, &dir));
    bool rtl = (dir == QChar::DirR);

    if(rtl)
    {
        line = RLE;
        line += LRE;
        line += "<font color=\"" + commandColor + "\">%2 %1" + PDF + " %3</font>";
    }
    else
    {
        if (!QApplication::isLeftToRight())
            line += LRE;

        line += "<font color=\"" + commandColor + "\">%1 %2 %3</font>";
    }

    line = line.arg(timeStamp(), prefix, text);

    emit textToLog(QString("%1\t%2").arg(type, message));

    doAppend(line, rtl, self);
}

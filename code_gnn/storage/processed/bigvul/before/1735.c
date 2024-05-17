void IRCView::appendAction(const QString& nick, const QString& message)
{
    QString actionColor = Preferences::self()->color(Preferences::ActionMessage).name();

    QString line;

    QString nickLine = createNickLine(nick, actionColor, false);

    if (message.isEmpty())
    {
        if (!QApplication::isLeftToRight())
            line += LRE;

        line += "<font color=\"" + actionColor + "\">%1 * " + nickLine + "</font>";

        line = line.arg(timeStamp(), nick);

        emit textToLog(QString("\t * %1").arg(nick));

        doAppend(line, false);
    }
    else
    {
        QChar::Direction dir;
        QString text(filter(message, actionColor, nick, true,true, false, &dir));
        bool rtl = (dir == QChar::DirR);

        if (rtl)
        {
            line = RLE;
            line += LRE;
            line += "<font color=\"" + actionColor + "\">" + nickLine + " * %1" + PDF + " %3</font>";
        }
        else
        {
            if (!QApplication::isLeftToRight())
                line += LRE;

            line += "<font color=\"" + actionColor + "\">%1 * " + nickLine + " %3</font>";
        }

        line = line.arg(timeStamp(), nick, text);

        emit textToLog(QString("\t * %1 %2").arg(nick, message));

        doAppend(line, rtl);
    }
}

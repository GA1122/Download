void IRCView::appendRaw(const QString& message, bool self)
{
    QColor color = self ? Preferences::self()->color(Preferences::ChannelMessage)
        : Preferences::self()->color(Preferences::ServerMessage);
    m_tabNotification = Konversation::tnfNone;

    QString line = QString(timeStamp() + " <font color=\"" + color.name() + "\">" + message + "</font>");

    doAppend(line, false, self);
}

QStringList IrcViewMimeData::formats() const
{
    if (!fragment.isEmpty())
        return QStringList() << QString::fromLatin1("text/plain");
    else
        return QMimeData::formats();
}

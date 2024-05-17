static inline QString toQString(JSStringRef string)
{
    return QString(reinterpret_cast<const QChar*>(JSStringGetCharactersPtr(string)), JSStringGetLength(string));
}

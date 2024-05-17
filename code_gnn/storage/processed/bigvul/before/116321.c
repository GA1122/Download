static inline QJSValue toQJSValue(JSStringRef string)
{
    return QJSValue(toQString(string));
}

static QString readUserScript(const QUrl& url)
{
    QString path;
    if (url.isLocalFile())
        path = url.toLocalFile();
    else if (url.scheme() == QLatin1String("qrc"))
        path = QStringLiteral(":") + url.path();
    else {
        qWarning("QQuickWebView: Couldn't open '%s' as user script because only file:/// and qrc:/// URLs are supported.", qPrintable(url.toString()));
        return QString();
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("QQuickWebView: Couldn't open '%s' as user script due to error '%s'.", qPrintable(url.toString()), qPrintable(file.errorString()));
        return QString();
    }

    QString contents = QString::fromUtf8(file.readAll());
    if (contents.isEmpty())
        qWarning("QQuickWebView: Ignoring '%s' as user script because file is empty.", qPrintable(url.toString()));

    return contents;
}

void QQuickWebViewPrivate::updateUserScripts()
{
    Vector<String> scripts;
    scripts.reserveCapacity(userScripts.size());

    for (unsigned i = 0; i < userScripts.size(); ++i) {
        const QUrl& url = userScripts.at(i);
        if (!url.isValid()) {
            qWarning("QQuickWebView: Couldn't open '%s' as user script because URL is invalid.", qPrintable(url.toString()));
            continue;
        }

        QString contents = readUserScript(url);
        if (contents.isEmpty())
            continue;
        scripts.append(String(contents));
    }

    webPageProxy->setUserScripts(scripts);
}

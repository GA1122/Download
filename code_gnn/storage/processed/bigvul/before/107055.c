void QQuickWebViewExperimental::schemeDelegates_Append(QDeclarativeListProperty<QQuickUrlSchemeDelegate>* property, QQuickUrlSchemeDelegate *scheme)
{
    QObject* schemeParent = property->object;
    scheme->setParent(schemeParent);
    QQuickWebViewExperimental* webViewExperimental = qobject_cast<QQuickWebViewExperimental*>(property->object->parent());
    if (!webViewExperimental)
        return;
    scheme->reply()->setWebViewExperimental(webViewExperimental);
    QQuickWebViewPrivate* d = webViewExperimental->d_func();
    d->webPageProxy->registerApplicationScheme(scheme->scheme());
}

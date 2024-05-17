void QQuickWebView::runJavaScriptInMainFrame(const QString &script, QObject *receiver, const char *method)
{
    Q_D(QQuickWebView);

    JSCallbackClosure* closure = new JSCallbackClosure;
    closure->receiver = receiver;
    closure->method = method;

    d->webPageProxy.get()->runJavaScriptInMainFrame(script, ScriptValueCallback::create(closure, javaScriptCallback));
}

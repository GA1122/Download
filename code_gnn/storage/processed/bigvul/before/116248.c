void QQuickWebViewExperimental::evaluateJavaScript(const QString& script, const QJSValue& value)
{
    JSCallbackClosure* closure = new JSCallbackClosure;

    closure->receiver = this;
    closure->value = value;

    d_ptr->webPageProxy.get()->runJavaScriptInMainFrame(script, ScriptValueCallback::create(closure, javaScriptCallback));
}

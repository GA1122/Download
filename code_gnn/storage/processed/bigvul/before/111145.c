bool WebPagePrivate::executeJavaScript(const BlackBerry::Platform::String& scriptUTF8, JavaScriptDataType& returnType, WebString& returnValue)
{
    BLACKBERRY_ASSERT(scriptUTF8.isUtf8());
    String script = scriptUTF8;

    if (script.isNull()) {
        returnType = JSException;
        return false;
    }

    if (script.isEmpty()) {
        returnType = JSUndefined;
        return true;
    }

    ScriptValue result = m_mainFrame->script()->executeScript(script, false);
    JSC::JSValue value = result.jsValue();
    if (!value) {
        returnType = JSException;
        return false;
    }

    if (value.isUndefined())
        returnType = JSUndefined;
    else if (value.isNull())
        returnType = JSNull;
    else if (value.isBoolean())
        returnType = JSBoolean;
    else if (value.isNumber())
        returnType = JSNumber;
    else if (value.isString())
        returnType = JSString;
    else if (value.isObject())
        returnType = JSObject;
    else
        returnType = JSUndefined;

    if (returnType == JSBoolean || returnType == JSNumber || returnType == JSString || returnType == JSObject) {
        JSC::ExecState* exec = m_mainFrame->script()->globalObject(mainThreadNormalWorld())->globalExec();
        returnValue = result.toString(exec);
    }

    return true;
}

bool WebPagePrivate::executeJavaScriptInIsolatedWorld(const ScriptSourceCode& sourceCode, JavaScriptDataType& returnType, BlackBerry::Platform::String& returnValue)
{
    if (!m_isolatedWorld)
        m_isolatedWorld = m_mainFrame->script()->createWorld();

    ScriptValue result = m_mainFrame->script()->evaluateInWorld(sourceCode, m_isolatedWorld.get());
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

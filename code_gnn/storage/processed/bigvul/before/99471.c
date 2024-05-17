bool NPJSObject::invokeDefault(const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;

    JSLock lock(SilenceAssertionsOnly);

    JSValue function = m_jsObject;
    return invoke(exec, m_objectMap->globalObject(), function, arguments, argumentCount, result);
}

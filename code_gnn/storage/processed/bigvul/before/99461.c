bool NPJSObject::construct(const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;

    JSLock lock(SilenceAssertionsOnly);

    ConstructData constructData;
    ConstructType constructType = getConstructData(m_jsObject, constructData);
    if (constructType == ConstructTypeNone)
        return false;

    MarkedArgumentBuffer argumentList;
    for (uint32_t i = 0; i < argumentCount; ++i)
        argumentList.append(m_objectMap->convertNPVariantToJSValue(exec, m_objectMap->globalObject(), arguments[i]));

    exec->globalData().timeoutChecker.start();
    JSValue value = JSC::construct(exec, m_jsObject, constructType, constructData, argumentList);
    exec->globalData().timeoutChecker.stop();
    
    m_objectMap->convertJSValueToNPVariant(exec, value, *result);
    exec->clearException();

    return true;
}

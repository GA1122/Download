bool NPJSObject::invoke(NPIdentifier methodName, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    IdentifierRep* identifierRep = static_cast<IdentifierRep*>(methodName);
    
    if (!identifierRep->isString())
        return false;
    
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;
    
    JSLock lock(SilenceAssertionsOnly);

    JSValue function = m_jsObject->get(exec, identifierFromIdentifierRep(exec, identifierRep));
    return invoke(exec, m_objectMap->globalObject(), function, arguments, argumentCount, result);
}

bool NPJSObject::hasMethod(NPIdentifier methodName)
{
    IdentifierRep* identifierRep = static_cast<IdentifierRep*>(methodName);

    if (!identifierRep->isString())
        return false;

    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;

    JSLock lock(SilenceAssertionsOnly);

    JSValue value = m_jsObject->get(exec, identifierFromIdentifierRep(exec, identifierRep));    
    exec->clearException();

    CallData callData;
    return getCallData(value, callData) != CallTypeNone;
}

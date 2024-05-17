bool NPJSObject::getProperty(NPIdentifier propertyName, NPVariant* result)
{
    IdentifierRep* identifierRep = static_cast<IdentifierRep*>(propertyName);
    
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;

    JSLock lock(SilenceAssertionsOnly);
    JSValue jsResult;
    if (identifierRep->isString())
        jsResult = m_jsObject->get(exec, identifierFromIdentifierRep(exec, identifierRep));
    else
        jsResult = m_jsObject->get(exec, identifierRep->number());
    
    m_objectMap->convertJSValueToNPVariant(exec, jsResult, *result);
    exec->clearException();
    return true;
}

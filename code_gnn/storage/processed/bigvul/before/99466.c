bool NPJSObject::hasProperty(NPIdentifier identifier)
{
    IdentifierRep* identifierRep = static_cast<IdentifierRep*>(identifier);
    
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;
    
    JSLock lock(SilenceAssertionsOnly);

    bool result;
    if (identifierRep->isString())
        result = m_jsObject->hasProperty(exec, identifierFromIdentifierRep(exec, identifierRep));
    else
        result = m_jsObject->hasProperty(exec, identifierRep->number());

    exec->clearException();
    return result;
}

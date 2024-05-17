bool NPJSObject::setProperty(NPIdentifier propertyName, const NPVariant* value)
{
    IdentifierRep* identifierRep = static_cast<IdentifierRep*>(propertyName);
    
    ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;
    
    JSLock lock(SilenceAssertionsOnly);

    JSValue jsValue = m_objectMap->convertNPVariantToJSValue(exec, m_objectMap->globalObject(), *value);
    if (identifierRep->isString()) {
        PutPropertySlot slot;
        m_jsObject->put(exec, identifierFromIdentifierRep(exec, identifierRep), jsValue, slot);
    } else
        m_jsObject->put(exec, identifierRep->number(), jsValue);
    exec->clearException();
    
     return true;
 }

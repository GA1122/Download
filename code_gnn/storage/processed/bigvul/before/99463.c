 bool NPJSObject::enumerate(NPIdentifier** identifiers, uint32_t* identifierCount)
 {
     ExecState* exec = m_objectMap->globalExec();
    if (!exec)
        return false;
    
    JSLock lock(SilenceAssertionsOnly);

    PropertyNameArray propertyNames(exec);
    m_jsObject->getPropertyNames(exec, propertyNames);

    NPIdentifier* nameIdentifiers = static_cast<NPIdentifier*>(malloc(sizeof(NPIdentifier) * propertyNames.size()));

    for (size_t i = 0; i < propertyNames.size(); ++i)
        nameIdentifiers[i] = static_cast<NPIdentifier>(IdentifierRep::get(propertyNames[i].ustring().UTF8String().data()));

    *identifiers = nameIdentifiers;
    *identifierCount = propertyNames.size();

    return true;
}

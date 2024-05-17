static bool NPN_HasProperty(NPP, NPObject* npObject, NPIdentifier propertyName)
{
    if (npObject->_class->hasProperty)
        return npObject->_class->hasProperty(npObject, propertyName);

    return false;
}

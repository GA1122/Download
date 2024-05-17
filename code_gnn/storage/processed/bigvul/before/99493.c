static bool NPN_HasMethod(NPP, NPObject* npObject, NPIdentifier methodName)
{
    if (npObject->_class->hasMethod)
        return npObject->_class->hasMethod(npObject, methodName);

    return false;
}

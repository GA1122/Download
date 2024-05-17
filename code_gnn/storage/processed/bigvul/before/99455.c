bool NPJSObject::NP_GetProperty(NPObject* npObject, NPIdentifier propertyName, NPVariant* result)
{
    return toNPJSObject(npObject)->getProperty(propertyName, result);
}

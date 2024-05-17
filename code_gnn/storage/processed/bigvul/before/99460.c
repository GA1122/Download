bool NPJSObject::NP_SetProperty(NPObject* npObject, NPIdentifier propertyName, const NPVariant* value)
{
     return toNPJSObject(npObject)->setProperty(propertyName, value);
 }

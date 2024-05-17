bool NPJSObject::NP_HasMethod(NPObject* npObject, NPIdentifier methodName)
{
    return toNPJSObject(npObject)->hasMethod(methodName);
}

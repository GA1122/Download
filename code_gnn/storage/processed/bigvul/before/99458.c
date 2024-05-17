bool NPJSObject::NP_Invoke(NPObject* npObject, NPIdentifier methodName, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    return toNPJSObject(npObject)->invoke(methodName, arguments, argumentCount, result);
}

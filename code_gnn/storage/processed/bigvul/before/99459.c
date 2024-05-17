bool NPJSObject::NP_InvokeDefault(NPObject* npObject, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    return toNPJSObject(npObject)->invokeDefault(arguments, argumentCount, result);
}

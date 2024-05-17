bool NPJSObject::NP_Construct(NPObject* npObject, const NPVariant* arguments, uint32_t argumentCount, NPVariant* result)
{
    return toNPJSObject(npObject)->construct(arguments, argumentCount, result);
}

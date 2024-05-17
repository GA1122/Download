bool NPJSObject::isNPJSObject(NPObject* npObject)
{
    return npObject->_class == npClass();
}

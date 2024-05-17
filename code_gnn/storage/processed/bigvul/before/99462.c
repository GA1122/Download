NPJSObject* NPJSObject::create(NPRuntimeObjectMap* objectMap, JSObject* jsObject)
{
    NPJSObject* npJSObject = toNPJSObject(createNPObject(0, npClass()));
    npJSObject->initialize(objectMap, jsObject);

    return npJSObject;
}

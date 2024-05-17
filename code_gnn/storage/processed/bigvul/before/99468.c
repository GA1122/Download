void NPJSObject::initialize(NPRuntimeObjectMap* objectMap, JSObject* jsObject)
{
    ASSERT(!m_objectMap);
    ASSERT(!m_jsObject);

    m_objectMap = objectMap;
    m_jsObject = jsObject;
}

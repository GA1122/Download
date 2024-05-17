NPJSObject::~NPJSObject()
{
    m_objectMap->npJSObjectDestroyed(this);
}

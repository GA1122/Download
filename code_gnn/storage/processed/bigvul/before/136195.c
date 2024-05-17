PageSerializer::PageSerializer(Vector<SerializedResource>* resources, PassOwnPtr<Delegate> delegate)
    : m_resources(resources)
    , m_blankFrameCounter(0)
    , m_delegate(delegate)
{
}

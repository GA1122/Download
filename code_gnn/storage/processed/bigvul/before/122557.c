void WebDevToolsAgentImpl::dumpUncountedAllocatedObjects(const HashMap<const void*, size_t>& map)
{
    class InstrumentedObjectSizeProvider : public WebDevToolsAgentClient::InstrumentedObjectSizeProvider {
    public:
        InstrumentedObjectSizeProvider(const HashMap<const void*, size_t>& map) : m_map(map) { }
        virtual size_t objectSize(const void* ptr) const
        {
            HashMap<const void*, size_t>::const_iterator i = m_map.find(ptr);
            return i == m_map.end() ? 0 : i->value;
        }

    private:
        const HashMap<const void*, size_t>& m_map;
    };

    InstrumentedObjectSizeProvider provider(map);
    m_client->dumpUncountedAllocatedObjects(&provider);
}

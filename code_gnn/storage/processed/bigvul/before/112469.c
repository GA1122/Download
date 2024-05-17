Element* Document::getElementByAccessKey(const String& key)
{
    if (key.isEmpty())
        return 0;
    if (!m_accessKeyMapValid) {
        buildAccessKeyMap(this);
        m_accessKeyMapValid = true;
    }
    return m_elementsByAccessKey.get(key.impl());
}

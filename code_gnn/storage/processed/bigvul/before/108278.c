void FrameLoader::setURL(const KURL& url)
{
    KURL ref(url);
    ref.setUser(String());
    ref.setPass(String());
    ref.removeFragmentIdentifier();
    m_outgoingReferrer = ref.string();
    m_URL = url;
}

void PageSerializer::registerRewriteURL(const String& from, const String& to)
{
    m_rewriteURLs.set(from, to);
}

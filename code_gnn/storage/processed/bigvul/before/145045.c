bool CSPSource::pathMatches(const KURL& url) const
{
    if (m_path.isEmpty())
        return true;

    String path = decodeURLEscapeSequences(url.path());

    if (m_path.endsWith("/"))
        return path.startsWith(m_path, TextCaseInsensitive);

    return path == m_path;
}

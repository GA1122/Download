KURL PageSerializer::urlForBlankFrame(LocalFrame* frame)
{
    BlankFrameURLMap::iterator iter = m_blankFrameURLs.find(frame);
    if (iter != m_blankFrameURLs.end())
        return iter->value;
    String url = "wyciwyg://frame/" + String::number(m_blankFrameCounter++);
    KURL fakeURL(ParsedURLString, url);
    m_blankFrameURLs.add(frame, fakeURL);

    return fakeURL;
}

WebFrame* WebFrame::top() const
{
    WebFrame* frame = const_cast<WebFrame*>(this);
    for (WebFrame* parent = frame; parent; parent = parent->m_parent)
        frame = parent;
    return frame;
}

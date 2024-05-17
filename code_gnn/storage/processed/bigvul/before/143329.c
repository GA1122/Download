void WebFrame::appendChild(WebFrame* child)
{
    insertAfter(child, m_lastChild);
}

WebPage* WebFrame::page() const
{ 
    if (!m_coreFrame)
        return 0;
    
    if (WebCore::Page* page = m_coreFrame->page())
        return static_cast<WebChromeClient*>(page->chrome()->client())->page();

    return 0;
}

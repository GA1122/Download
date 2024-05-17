bool WebFrame::isMainFrame() const
{
    if (WebPage* p = page())
        return p->mainFrame() == this;

    return false;
}

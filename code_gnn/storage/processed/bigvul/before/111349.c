void WebPage::setForcedTextEncoding(const BlackBerry::Platform::String& encoding)
{
    if (!encoding.empty() && d->focusedOrMainFrame() && d->focusedOrMainFrame()->loader() && d->focusedOrMainFrame()->loader())
        return d->focusedOrMainFrame()->loader()->reloadWithOverrideEncoding(encoding);
}

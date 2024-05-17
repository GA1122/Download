BlackBerry::Platform::String WebPage::textEncoding()
{
    Frame* frame = d->focusedOrMainFrame();
    if (!frame)
        return "";

    Document* document = frame->document();
    if (!document)
        return "";

    return document->loader()->writer()->encoding();
}

BlackBerry::Platform::String WebPage::forcedTextEncoding()
{
    Frame* frame = d->focusedOrMainFrame();
    if (!frame)
        return BlackBerry::Platform::String::emptyString();

    Document* document = frame->document();
    if (!document)
        return BlackBerry::Platform::String::emptyString();

    return document->loader()->overrideEncoding();
}

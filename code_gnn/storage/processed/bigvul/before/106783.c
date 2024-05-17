void WebPage::confirmComposition(const String& compositionString)
{
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    if (!frame || !frame->editor()->canEdit())
        return;
    frame->editor()->confirmComposition(compositionString);
}

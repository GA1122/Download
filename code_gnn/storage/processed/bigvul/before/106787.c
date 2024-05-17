void WebPage::getSelectedText(String& text)
{
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    RefPtr<Range> selectedRange = frame->selection()->toNormalizedRange();
    text = selectedRange->text();
}

void WebPage::firstRectForCharacterInSelectedRange(const uint64_t characterPosition, WebCore::IntRect& resultRect)
{
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    IntRect rect;
    if (RefPtr<Range> range = frame->editor()->hasComposition() ? frame->editor()->compositionRange() : frame->selection()->selection().toNormalizedRange()) {
        ExceptionCode ec = 0;
        RefPtr<Range> tempRange = range->cloneRange(ec);
        tempRange->setStart(tempRange->startContainer(ec), tempRange->startOffset(ec) + characterPosition, ec);
        rect = frame->editor()->firstRectForRange(tempRange.get());
    }
    resultRect = frame->view()->contentsToWindow(rect);
}

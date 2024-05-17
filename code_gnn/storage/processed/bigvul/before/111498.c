void InputHandler::requestSpellingCheckingOptions(imf_sp_text_t& spellCheckingOptionRequest, WebCore::IntSize& screenOffset, const bool shouldMoveDialog)
{
    if (m_webPage->focusedOrMainFrame()->selection()->selectionType() != VisibleSelection::CaretSelection)
        return;

    if (!m_currentFocusElement || !m_currentFocusElement->document() || !m_currentFocusElement->document()->frame())
        return;

    if (shouldMoveDialog || !(spellCheckingOptionRequest.startTextPosition || spellCheckingOptionRequest.startTextPosition)) {
        if (m_spellCheckingOptionsRequest.startTextPosition || m_spellCheckingOptionsRequest.endTextPosition)
            spellCheckingOptionRequest = m_spellCheckingOptionsRequest;
    }

    if (!shouldMoveDialog && spellCheckingOptionRequest.startTextPosition == spellCheckingOptionRequest.endTextPosition)
        return;

    if (screenOffset.isEmpty()) {
        screenOffset.setWidth(m_screenOffset.width());
        screenOffset.setHeight(m_screenOffset.height());
    } else {
        m_screenOffset.setWidth(screenOffset.width());
        m_screenOffset.setHeight(screenOffset.height());
    }

    WebCore::IntRect caretRect = m_webPage->focusedOrMainFrame()->selection()->selection().visibleStart().absoluteCaretBounds();

    caretRect = m_webPage->focusedOrMainFrame()->view()->contentsToRootView(caretRect);

    const WebCore::IntPoint scrollPosition = m_webPage->mainFrame()->view()->scrollPosition();
    caretRect.move(scrollPosition.x(), scrollPosition.y());

    if (!shouldMoveDialog) {
        VisiblePosition caretPosition = m_currentFocusElement->document()->frame()->selection()->selection().visibleStart();

        RefPtr<Range> rangeSelection = VisibleSelection(startOfWord(caretPosition), endOfWord(caretPosition)).toNormalizedRange();
        if (!rangeSelection)
            return;

        unsigned location = 0;
        unsigned length = 0;
        TextIterator::getLocationAndLengthFromRange(m_currentFocusElement.get(), rangeSelection.get(), location, length);

        if (location != notFound && length) {
            spellCheckingOptionRequest.startTextPosition = location;
            spellCheckingOptionRequest.endTextPosition = location + length;
        }
    }
    m_spellCheckingOptionsRequest = spellCheckingOptionRequest;

    InputLog(LogLevelInfo, "InputHandler::requestSpellingCheckingOptions caretRect topLeft=(%d,%d), bottomRight=(%d,%d), startTextPosition=%d, endTextPosition=%d"
                            , caretRect.minXMinYCorner().x(), caretRect.minXMinYCorner().y(), caretRect.maxXMaxYCorner().x(), caretRect.maxXMaxYCorner().y()
                            , spellCheckingOptionRequest.startTextPosition, spellCheckingOptionRequest.endTextPosition);

    m_webPage->m_client->requestSpellingCheckingOptions(spellCheckingOptionRequest, caretRect, screenOffset, shouldMoveDialog);
}

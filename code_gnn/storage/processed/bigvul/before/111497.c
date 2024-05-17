void InputHandler::requestCheckingOfString(PassRefPtr<WebCore::TextCheckingRequest> textCheckingRequest)
{
    m_request = textCheckingRequest;

    InputLog(LogLevelInfo, "InputHandler::requestCheckingOfString '%s'", m_request->text().latin1().data());

    if (!m_request) {
        SpellingLog(LogLevelWarn, "InputHandler::requestCheckingOfString did not receive a valid request.");
        return;
    }

    unsigned requestLength = m_request->text().length();

    if (!isActiveTextEdit() || !shouldSpellCheckElement(m_currentFocusElement.get()) || requestLength < 2) {
        m_request->didCancel();
        return;
    }

    if (requestLength > MaxSpellCheckingStringLength) {
        m_request->didCancel();
        if (m_currentFocusElement->document() && m_currentFocusElement->document()->frame() && m_currentFocusElement->document()->frame()->selection()) {
            VisiblePosition caretPosition = m_currentFocusElement->document()->frame()->selection()->start();
            VisibleSelection visibleSelection = VisibleSelection(previousLinePosition(caretPosition, caretPosition.lineDirectionPointForBlockDirectionNavigation()), caretPosition);
            spellCheckBlock(visibleSelection, TextCheckingProcessIncremental);
        }
        return;
    }

    wchar_t* checkingString = (wchar_t*)malloc(sizeof(wchar_t) * (requestLength + 1));
    if (!checkingString) {
        logAlways(LogLevelCritical, "InputHandler::requestCheckingOfString Cannot allocate memory for string.");
        m_request->didCancel();
        return;
    }

    int paragraphLength = 0;
    if (!convertStringToWchar(m_request->text(), checkingString, requestLength + 1, &paragraphLength)) {
        logAlways(LogLevelCritical, "InputHandler::requestCheckingOfString Failed to convert String to wchar type.");
        free(checkingString);
        m_request->didCancel();
        return;
    }

    m_processingTransactionId = m_webPage->m_client->checkSpellingOfStringAsync(checkingString, static_cast<unsigned>(paragraphLength));
    free(checkingString);

    if (m_processingTransactionId == -1) {  
        m_request->didCancel();
        return;
    }
}

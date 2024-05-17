void InputHandler::setElementFocused(Element* element)
{
    ASSERT(DOMSupport::isTextBasedContentEditableElement(element));
    ASSERT(element && element->document() && element->document()->frame());

#ifdef ENABLE_SPELLING_LOG
    BlackBerry::Platform::StopWatch timer;
    timer.start();
#endif

    if (!element || !(element->document()))
        return;

    Frame* frame = element->document()->frame();
    if (!frame)
        return;

    if (frame->selection()->isFocused() != isInputModeEnabled())
        frame->selection()->setFocused(isInputModeEnabled());

    m_shouldEnsureFocusTextElementVisibleOnSelectionChanged = isActiveTextEdit();

    setElementUnfocused(true  );

    m_currentFocusElement = element;
    m_currentFocusElementType = TextEdit;

    BlackBerryInputType type = elementType(element);
    m_currentFocusElementTextEditMask = inputStyle(type, element);

    VirtualKeyboardType keyboardType = keyboardTypeAttribute(element);
    if (keyboardType == VKBTypeNotSet)
        keyboardType = convertInputTypeToVKBType(type);

    VirtualKeyboardEnterKeyType enterKeyType = keyboardEnterKeyTypeAttribute(element);

    if (enterKeyType == VKBEnterKeyNotSet && type != InputTypeTextArea) {
        if (element->isFormControlElement()) {
            const HTMLFormControlElement* formElement = static_cast<const HTMLFormControlElement*>(element);
            if (formElement->form() && formElement->form()->defaultButton())
                enterKeyType = VKBEnterKeySubmit;
        }
    }

    FocusLog(LogLevelInfo, "InputHandler::setElementFocused, Type=%d, Style=%d, Keyboard Type=%d, Enter Key=%d", type, m_currentFocusElementTextEditMask, keyboardType, enterKeyType);
    m_webPage->m_client->inputFocusGained(m_currentFocusElementTextEditMask, keyboardType, enterKeyType);

    handleInputLocaleChanged(m_webPage->m_webSettings->isWritingDirectionRTL());

    if (!m_delayKeyboardVisibilityChange)
        notifyClientOfKeyboardVisibilityChange(true, true  );

#ifdef ENABLE_SPELLING_LOG
    SpellingLog(LogLevelInfo, "InputHandler::setElementFocused Focusing the field took %f seconds.", timer.elapsed());
#endif

    if (!shouldSpellCheckElement(element))
        return;

    VisibleSelection focusedBlock = DOMSupport::visibleSelectionForInputElement(element);
    spellCheckBlock(focusedBlock, TextCheckingProcessBatch);

#ifdef ENABLE_SPELLING_LOG
    SpellingLog(LogLevelInfo, "InputHandler::setElementFocused Spellchecking the field increased the total time to focus to %f seconds.", timer.elapsed());
#endif
}

bool InputHandler::handleKeyboardInput(const Platform::KeyboardEvent& keyboardEvent, bool changeIsPartOfComposition)
{
    InputLog(LogLevelInfo, "InputHandler::handleKeyboardInput received character='%c', type=%d", keyboardEvent.character(), keyboardEvent.type());

    m_receivedBackspaceKeyDown = false;

    setInputModeEnabled();

    Platform::KeyboardEvent::Type type = keyboardEvent.type();
     
    if (type == Platform::KeyboardEvent::KeyUp) {
        if (m_expectedKeyUpChar == keyboardEvent.character() || (isASCIIUpper(m_expectedKeyUpChar) && m_expectedKeyUpChar == toASCIIUpper(keyboardEvent.character()))) {
            m_expectedKeyUpChar = 0;
            changeIsPartOfComposition = true;
        }
    }

    if (!changeIsPartOfComposition && compositionActive())
        return false;

    ProcessingChangeGuard guard(this);

    unsigned adjustedModifiers = keyboardEvent.modifiers();
    if (WTF::isASCIIUpper(keyboardEvent.character()))
        adjustedModifiers |= KEYMOD_SHIFT;

    ASSERT(m_webPage->m_page->focusController());
    bool keyboardEventHandled = false;
    if (Frame* focusedFrame = m_webPage->m_page->focusController()->focusedFrame()) {
        bool isKeyChar = type == Platform::KeyboardEvent::KeyChar;

        if (isKeyChar)
            type = Platform::KeyboardEvent::KeyDown;
        else if (type == Platform::KeyboardEvent::KeyDown) {
            m_expectedKeyUpChar = keyboardEvent.character();

            if (keyboardEvent.character() == KEYCODE_BACKSPACE)
                m_receivedBackspaceKeyDown = true;
        }

        Platform::KeyboardEvent adjustedKeyboardEvent(keyboardEvent.character(), type, adjustedModifiers);
        keyboardEventHandled = focusedFrame->eventHandler()->keyEvent(PlatformKeyboardEvent(adjustedKeyboardEvent));

        m_receivedBackspaceKeyDown = false;

        if (isKeyChar) {
            type = Platform::KeyboardEvent::KeyUp;
            adjustedKeyboardEvent = Platform::KeyboardEvent(keyboardEvent.character(), type, adjustedModifiers);
            keyboardEventHandled = focusedFrame->eventHandler()->keyEvent(PlatformKeyboardEvent(adjustedKeyboardEvent)) || keyboardEventHandled;
        }

        if (!changeIsPartOfComposition && type == Platform::KeyboardEvent::KeyUp)
            ensureFocusTextElementVisible(EdgeIfNeeded);
    }
    return keyboardEventHandled;
}

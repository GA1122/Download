void InputHandler::frameUnloaded(const Frame* frame)
{
    if (!isActiveTextEdit())
        return;

    if (m_currentFocusElement->document()->frame() != frame)
        return;

    FocusLog(LogLevelInfo, "InputHandler::frameUnloaded");

    setElementUnfocused(false  );
}

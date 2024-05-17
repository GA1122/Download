bool InputHandler::deleteTextRelativeToCursor(int leftOffset, int rightOffset)
{
    if (!isActiveTextEdit() || compositionActive())
        return false;

    ProcessingChangeGuard guard(this);

    InputLog(LogLevelInfo, "InputHandler::deleteTextRelativeToCursor left %d right %d", leftOffset, rightOffset);

    int caretOffset = caretPosition();
    int start = relativeLeftOffset(caretOffset, leftOffset);
    int end = relativeRightOffset(caretOffset, elementText().length(), rightOffset);

    if (leftOffset == 1 && !rightOffset) {
        if (!handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_BACKSPACE, Platform::KeyboardEvent::KeyDown, 0), true  ))
            return false;
    } else if (!deleteText(start, end))
        return false;

    ensureFocusTextElementVisible(EdgeIfNeeded);

    return true;
}

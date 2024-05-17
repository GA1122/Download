bool InputHandler::deleteText(int start, int end)
{
    if (!isActiveTextEdit())
        return false;

    ProcessingChangeGuard guard(this);

    if (end - start == 1)
        return handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_BACKSPACE, Platform::KeyboardEvent::KeyDown, 0), true  );

    if (!setSelection(start, end, true  ))
        return false;

    InputLog(LogLevelInfo, "InputHandler::deleteText start %d end %d", start, end);

    return deleteSelection();
}

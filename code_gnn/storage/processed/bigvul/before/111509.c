int32_t InputHandler::setComposingText(spannable_string_t* spannableString, int32_t relativeCursorPosition)
{
    if (!isActiveTextEdit())
        return -1;

    if (!spannableString)
        return -1;

    InputLog(LogLevelInfo, "InputHandler::setComposingText at relativeCursorPosition: %d", relativeCursorPosition);

    setInputModeEnabled();

    return setSpannableTextAndRelativeCursor(spannableString, relativeCursorPosition, true  ) ? 0 : -1;
}

int32_t InputHandler::commitText(spannable_string_t* spannableString, int32_t relativeCursorPosition)
{
    if (!isActiveTextEdit())
        return -1;

    if (!spannableString)
        return -1;

    InputLog(LogLevelInfo, "InputHandler::commitText");

    return setSpannableTextAndRelativeCursor(spannableString, relativeCursorPosition, false  ) ? 0 : -1;
}

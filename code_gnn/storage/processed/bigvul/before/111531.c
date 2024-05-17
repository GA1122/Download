spannable_string_t* InputHandler::textBeforeCursor(int32_t length, int32_t flags)
{
    if (!isActiveTextEdit())
        return 0;

    int caretOffset = caretPosition();
    int start = relativeLeftOffset(caretOffset, length);
    int end = caretOffset;

    return spannableTextInRange(start, end, flags);
}

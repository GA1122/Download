spannable_string_t* InputHandler::textAfterCursor(int32_t length, int32_t flags)
{
    if (!isActiveTextEdit())
        return 0;

    int caretOffset = caretPosition();
    int start = caretOffset;
    int end = relativeRightOffset(caretOffset, elementText().length(), length);

    return spannableTextInRange(start, end, flags);
}

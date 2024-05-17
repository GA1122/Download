spannable_string_t* InputHandler::selectedText(int32_t flags)
{
    if (!isActiveTextEdit())
        return 0;

    return spannableTextInRange(selectionStart(), selectionEnd(), flags);
}

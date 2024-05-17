bool InputHandler::setSpannableTextAndRelativeCursor(spannable_string_t* spannableString, int relativeCursorPosition, bool markTextAsComposing)
{
    InputLog(LogLevelInfo, "InputHandler::setSpannableTextAndRelativeCursor(%d, %d, %d)", spannableString->length, relativeCursorPosition, markTextAsComposing);
    int insertionPoint = compositionActive() ? m_composingTextStart : selectionStart();

    ProcessingChangeGuard guard(this);

    if (!setText(spannableString))
        return false;

    if (!setTextAttributes(insertionPoint, spannableString))
        return false;

    if (!setRelativeCursorPosition(insertionPoint, relativeCursorPosition))
        return false;

    if (markTextAsComposing) {
        m_composingTextStart = insertionPoint;
        m_composingTextEnd = insertionPoint + spannableString->length;
    }

    return true;
}

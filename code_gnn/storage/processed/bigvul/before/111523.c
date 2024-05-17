bool InputHandler::setText(spannable_string_t* spannableString)
{
    if (!isActiveTextEdit() || !spannableString)
        return false;

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());
    Frame* frame = m_currentFocusElement->document()->frame();

    Editor* editor = frame->editor();
    ASSERT(editor);

    m_webPage->m_selectionHandler->setSelectionActive(false);

    WTF::String textToInsert = convertSpannableStringToString(spannableString);
    int textLength = textToInsert.length();

    InputLog(LogLevelInfo, "InputHandler::setText spannableString is '%s', of length %d", textToInsert.latin1().data(), textLength);

    span_t* changedSpan = firstSpanInString(spannableString, CHANGED_ATTRIB);
    int composingTextStart = m_composingTextStart;
    int composingTextEnd = m_composingTextEnd;
    int composingTextLength = compositionLength();
    removeAttributedTextMarker();

    if (isTrailingSingleCharacter(changedSpan, textLength, composingTextLength)) {
        if (firstSpanInString(spannableString, UNCONVERTED_TEXT_ATTRIB)) {
            InputLog(LogLevelInfo, "InputHandler::setText Single trailing character detected.  Text is unconverted.");
            return editor->command("InsertText").execute(textToInsert.right(1));
        }
        InputLog(LogLevelInfo, "InputHandler::setText Single trailing character detected.");
        return handleKeyboardInput(Platform::KeyboardEvent(textToInsert[textLength - 1], Platform::KeyboardEvent::KeyDown, 0), false  );
    }

    if (!changedSpan) {
        if (composingTextLength == textLength) {
            InputLog(LogLevelInfo, "InputHandler::setText No spans have changed. New text is the same length as the old. Nothing to do.");
            return true;
        }

        if (composingTextLength - textLength == 1) {
            InputLog(LogLevelInfo, "InputHandler::setText No spans have changed. New text is one character shorter than the old. Treating as 'delete'.");
            return handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_BACKSPACE, Platform::KeyboardEvent::KeyDown, 0), true  );
        }
    }

    if (composingTextLength && !setSelection(composingTextStart, composingTextEnd, true  ))
        return false;

    if (!textLength) {
        if (selectionActive())
            return handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_BACKSPACE, Platform::KeyboardEvent::KeyDown, 0), true  );

        return true;
    }

    bool requiresSpaceKeyPress = false;
    if (textLength > 0 && textToInsert[textLength - 1] == KEYCODE_SPACE) {
        requiresSpaceKeyPress = true;
        textLength--;
        textToInsert.remove(textLength, 1);
    }

    InputLog(LogLevelInfo, "InputHandler::setText Request being processed. Text before processing: '%s'", elementText().latin1().data());

    if (textLength == 1 && !spannableString->spans_count) {
        InputLog(LogLevelInfo, "InputHandler::setText Single character entry treated as key-press in the absense of spans.");
        return handleKeyboardInput(Platform::KeyboardEvent(textToInsert[0], Platform::KeyboardEvent::KeyDown, 0), true  );
    }

    if (!textToInsert.isEmpty() && !editor->command("InsertText").execute(textToInsert)) {
        InputLog(LogLevelWarn, "InputHandler::setText Failed to insert text '%s'", textToInsert.latin1().data());
        return false;
    }

    if (requiresSpaceKeyPress)
        handleKeyboardInput(Platform::KeyboardEvent(KEYCODE_SPACE, Platform::KeyboardEvent::KeyDown, 0), true  );

    InputLog(LogLevelInfo, "InputHandler::setText Request being processed. Text after processing '%s'", elementText().latin1().data());

    return true;
}

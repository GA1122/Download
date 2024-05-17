bool InputHandler::setTextAttributes(int insertionPoint, spannable_string_t* spannableString)
{
    span_t* span = spannableString->spans;
    for (unsigned int i = 0; i < spannableString->spans_count; i++) {
        unsigned int startPosition = insertionPoint + span->start;
        unsigned int endPosition = insertionPoint + span->end + 1;
        if (endPosition < startPosition || endPosition > elementText().length())
            return false;

        if (!span->attributes_mask)
            continue;  


        InputLog(LogLevelInfo, "InputHandler::setTextAttributes adding marker %d to %d - %llu", startPosition, endPosition, span->attributes_mask);
        addAttributedTextMarker(startPosition, endPosition, textStyleFromMask(span->attributes_mask));

        span++;
    }

    InputLog(LogLevelInfo, "InputHandler::setTextAttributes attribute count %d", spannableString->spans_count);

    return true;
}

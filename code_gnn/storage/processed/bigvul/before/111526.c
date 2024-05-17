spannable_string_t* InputHandler::spannableTextInRange(int start, int end, int32_t flags)
{
    if (!isActiveTextEdit())
        return 0;

    if (start >= end)
        return 0;

    int length = end - start;

    WTF::String textString = elementText().substring(start, length);

    spannable_string_t* pst = (spannable_string_t*)fastMalloc(sizeof(spannable_string_t));

    pst->str = (wchar_t*)malloc(sizeof(wchar_t) * (length + 1));
    if (!pst->str) {
        logAlways(LogLevelCritical, "InputHandler::spannableTextInRange Cannot allocate memory for string.");
        free(pst);
        return 0;
    }

    int stringLength = 0;
    if (!convertStringToWchar(textString, pst->str, length + 1, &stringLength)) {
        logAlways(LogLevelCritical, "InputHandler::spannableTextInRange failed to convert string.");
        free(pst->str);
        free(pst);
        return 0;
    }

    pst->length = stringLength;
    pst->spans_count = 0;
    pst->spans = 0;

    return pst;
}

void InputHandler::spellCheckingRequestProcessed(int32_t transactionId, spannable_string_t* spannableString)
{
    SpellingLog(LogLevelWarn, "InputHandler::spellCheckingRequestProcessed Expected transaction id %d, received %d. %s"
                , transactionId
                , m_processingTransactionId
                , transactionId == m_processingTransactionId ? "" : "We are out of sync with input service.");

    if (!spannableString || !isActiveTextEdit()) {
        SpellingLog(LogLevelWarn, "InputHandler::spellCheckingRequestProcessed Cancelling request with transactionId %d.", transactionId);
        m_request->didCancel();
        m_processingTransactionId = -1;
        return;
    }

    Vector<TextCheckingResult> results;

    WTF::String replacement;
    TextCheckingResult textCheckingResult;
    textCheckingResult.type = TextCheckingTypeSpelling;
    textCheckingResult.replacement = replacement;
    textCheckingResult.location = 0;
    textCheckingResult.length = 0;

    span_t* span = spannableString->spans;
    for (unsigned int i = 0; i < spannableString->spans_count; i++) {
        if (!span)
            break;
        if (span->end < span->start) {
            m_request->didCancel();
            return;
        }
        if (span->attributes_mask & MISSPELLED_WORD_ATTRIB) {
            textCheckingResult.location = span->start;
            textCheckingResult.length = span->end - span->start + 1;
            results.append(textCheckingResult);
        }
        span++;
    }

    free(spannableString->spans);
    free(spannableString);

    m_request->didSucceed(results);
}

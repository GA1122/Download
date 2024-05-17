span_t* InputHandler::firstSpanInString(spannable_string_t* spannableString, SpannableStringAttribute attrib)
{
    span_t* span = spannableString->spans;
    for (unsigned int i = 0; i < spannableString->spans_count; i++) {
        if (span->attributes_mask & attrib)
            return span;
        span++;
    }

    return 0;
}

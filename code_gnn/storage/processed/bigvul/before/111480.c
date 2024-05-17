bool InputHandler::isTrailingSingleCharacter(span_t* span, unsigned stringLength, unsigned composingTextLength)
{
    if (composingTextLength != stringLength - 1)
        return false;

    if (!span)
        return false;

    if (span->start == span->end) {
        if (span->start == stringLength - 1)
            return true;
    }
    return false;
}

int32_t InputHandler::setComposingRegion(int32_t start, int32_t end)
{
    if (!isActiveTextEdit())
        return -1;

    if (!removeComposedText()) {
        return -1;
    }

    m_composingTextStart = start;
    m_composingTextEnd = end;

    if (compositionActive())
        addAttributedTextMarker(start, end, compositionTextStyle());

    InputLog(LogLevelInfo, "InputHandler::setComposingRegion start %d end %d", start, end);

    return 0;
}

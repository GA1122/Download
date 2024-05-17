void InputHandler::spellCheckBlock(VisibleSelection& visibleSelection, TextCheckingProcessType textCheckingProcessType)
{
    if (!isActiveTextEdit())
        return;

    RefPtr<Range> rangeForSpellChecking = visibleSelection.toNormalizedRange();
    if (!rangeForSpellChecking || !rangeForSpellChecking->text() || !rangeForSpellChecking->text().length())
        return;

    SpellChecker* spellChecker = getSpellChecker();
    if (!spellChecker) {
        SpellingLog(LogLevelInfo, "InputHandler::spellCheckBlock Failed to spellcheck the current focused element.");
        return;
    }

    if (textCheckingProcessType == TextCheckingProcessBatch) {
        if (rangeForSpellChecking->text().length() < MaxSpellCheckingStringLength) {
            spellChecker->requestCheckingFor(SpellCheckRequest::create(TextCheckingTypeSpelling, TextCheckingProcessBatch, rangeForSpellChecking, rangeForSpellChecking));
            return;
        }
    }

    VisiblePosition startPos = visibleSelection.visibleStart();
    VisiblePosition startOfCurrentLine = startOfLine(startPos);
    VisiblePosition endOfCurrentLine = endOfLine(startOfCurrentLine);

    while (!isEndOfBlock(startOfCurrentLine)) {
        rangeForSpellChecking = VisibleSelection(startOfCurrentLine, endOfCurrentLine).toNormalizedRange();

        if (rangeForSpellChecking->text().length() < MaxSpellCheckingStringLength) {
            startOfCurrentLine = nextLinePosition(startOfCurrentLine, startOfCurrentLine.lineDirectionPointForBlockDirectionNavigation());
            endOfCurrentLine = endOfLine(startOfCurrentLine);
        } else {
            rangeForSpellChecking = getRangeForSpellCheckWithFineGranularity(startOfCurrentLine, endOfCurrentLine);
            if (!rangeForSpellChecking) {
                SpellingLog(LogLevelWarn, "InputHandler::spellCheckBlock Failed to set text range for spellchecking");
                return;
            }
            startOfCurrentLine = VisiblePosition(rangeForSpellChecking->endPosition());
            endOfCurrentLine = endOfLine(startOfCurrentLine);
            rangeForSpellChecking = DOMSupport::trimWhitespaceFromRange(VisiblePosition(rangeForSpellChecking->startPosition()), VisiblePosition(rangeForSpellChecking->endPosition()));
        }

        SpellingLog(LogLevelInfo, "InputHandler::spellCheckBlock Substring text is '%s', of size %d", rangeForSpellChecking->text().latin1().data(), rangeForSpellChecking->text().length());

        spellChecker->requestCheckingFor(SpellCheckRequest::create(TextCheckingTypeSpelling, TextCheckingProcessBatch, rangeForSpellChecking, rangeForSpellChecking));
    }
}

PassRefPtr<Range> InputHandler::getRangeForSpellCheckWithFineGranularity(VisiblePosition startPosition, VisiblePosition endPosition)
{
    VisiblePosition endOfCurrentWord = endOfWord(startPosition);

    while (startPosition != endPosition) {
        if (VisibleSelection(startPosition, endOfCurrentWord).toNormalizedRange()->text().length() >= MaxSpellCheckingStringLength) {
            if (startOfWord(endOfCurrentWord, LeftWordIfOnBoundary) != startPosition && !DOMSupport::isEmptyRangeOrAllSpaces(startPosition, endOfCurrentWord))
                return VisibleSelection(startPosition, endOfWord(previousWordPosition(endOfCurrentWord), LeftWordIfOnBoundary)).toNormalizedRange();

            startPosition = endOfCurrentWord;
            endOfCurrentWord = endOfWord(nextWordPosition(endOfCurrentWord));
        } else if (endOfCurrentWord == endPosition) {
            return VisibleSelection(startPosition, endPosition).toNormalizedRange();
        } else {
            endOfCurrentWord = endOfWord(nextWordPosition(endOfCurrentWord));
        }
    }
    return 0;
}

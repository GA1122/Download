static inline bool collectCandidateRuns(const UChar* normalizedBuffer,
    size_t bufferLength, const Font* font, Vector<CandidateRun>* runs, bool isSpaceNormalize)
{
    UTF16TextIterator iterator(normalizedBuffer, bufferLength);
    UChar32 character;
    unsigned startIndexOfCurrentRun = 0;

    if (!iterator.consume(character))
        return false;

    const SimpleFontData* nextFontData = font->glyphDataForCharacter(character, false, isSpaceNormalize).fontData;
    UErrorCode errorCode = U_ZERO_ERROR;
    UScriptCode nextScript = uscript_getScript(character, &errorCode);
    if (U_FAILURE(errorCode))
        return false;

    do {
        const UChar* currentCharacterPosition = iterator.characters();
        const SimpleFontData* currentFontData = nextFontData;
        UScriptCode currentScript = nextScript;

        UChar32 lastCharacter = character;
        for (iterator.advance(); iterator.consume(character); iterator.advance()) {
            if (Character::treatAsZeroWidthSpace(character))
                continue;
            if ((U_GET_GC_MASK(character) & U_GC_M_MASK)
                && currentFontData->canRenderCombiningCharacterSequence(
                    currentCharacterPosition,
                    iterator.glyphEnd() - currentCharacterPosition))
                continue;

            nextFontData = font->glyphDataForCharacter(character, false, isSpaceNormalize).fontData;
            nextScript = uscript_getScript(character, &errorCode);
            if (U_FAILURE(errorCode))
                return false;
            if (lastCharacter == zeroWidthJoiner)
                currentFontData = nextFontData;
            if ((nextFontData != currentFontData) || ((currentScript != nextScript) && (nextScript != USCRIPT_INHERITED) && (!uscript_hasScript(character, currentScript))))
                break;
            currentCharacterPosition = iterator.characters();
            lastCharacter = character;
        }

        CandidateRun run = { character, startIndexOfCurrentRun, iterator.offset(), currentFontData, currentScript };
        runs->append(run);

        startIndexOfCurrentRun = iterator.offset();
    } while (iterator.consume(character));

    return true;
}

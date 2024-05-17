static void normalizeCharacters(const TextRun& run, unsigned length, UChar* destination, unsigned* destinationLength)
{
    unsigned position = 0;
    bool error = false;
    const UChar* source;
    String stringFor8BitRun;
    if (run.is8Bit()) {
        stringFor8BitRun = String::make16BitFrom8BitSource(run.characters8(), run.length());
        source = stringFor8BitRun.characters16();
    } else {
        source = run.characters16();
    }

    *destinationLength = 0;
    while (position < length) {
        UChar32 character;
        U16_NEXT(source, position, length, character);
        if (run.normalizeSpace() && Character::isNormalizedCanvasSpaceCharacter(character))
            character = space;
        else if (Character::treatAsSpace(character) && character != characterTabulation)
            character = space;
        else if (Character::treatAsZeroWidthSpaceInComplexScript(character))
            character = zeroWidthSpace;

        U16_APPEND(destination, *destinationLength, length, character, error);
        ASSERT_UNUSED(error, !error);
    }
}

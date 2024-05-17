int relativeRightOffset(int caretPosition, unsigned totalLengthOfText, int rightOffset)
{
    ASSERT(caretPosition >= 0);
    ASSERT(totalLengthOfText < INT_MAX);

    return std::min(caretPosition + rightOffset, static_cast<int>(totalLengthOfText));
}

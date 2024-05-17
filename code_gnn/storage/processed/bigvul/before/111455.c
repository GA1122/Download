static bool convertStringToWcharVector(const WTF::String& string, WTF::Vector<wchar_t>& wcharString)
{
    ASSERT(wcharString.isEmpty());

    int length = string.length();
    if (!length)
        return true;

    if (!wcharString.tryReserveCapacity(length + 1)) {
        logAlways(LogLevelCritical, "InputHandler::convertStringToWcharVector Cannot allocate memory for string.");
        return false;
    }

    int destLength = 0;
    if (!convertStringToWchar(string, wcharString.data(), length + 1, &destLength))
        return false;

    wcharString.resize(destLength);
    return true;
}

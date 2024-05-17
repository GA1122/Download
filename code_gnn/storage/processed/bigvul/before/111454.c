static bool convertStringToWchar(const WTF::String& string, wchar_t* dest, int destCapacity, int* destLength)
{
    ASSERT(dest);

    int length = string.length();

    if (!length) {
        destLength = 0;
        return true;
    }

    UErrorCode ec = U_ZERO_ERROR;

    u_strToUTF32(reinterpret_cast<UChar32*>(dest), destCapacity, destLength, string.characters(), length, &ec);
    if (ec) {
        logAlways(LogLevelCritical, "InputHandler::convertStringToWchar Error converting string ec (%d).", ec);
        destLength = 0;
        return false;
    }
    return true;
}

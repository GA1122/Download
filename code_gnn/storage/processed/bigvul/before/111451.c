static WTF::String convertSpannableStringToString(spannable_string_t* src)
{
    if (!src || !src->str || !src->length)
        return WTF::String();

    WTF::Vector<UChar> dest;
    int destCapacity = (src->length * 2) + 1;
    if (!dest.tryReserveCapacity(destCapacity)) {
        logAlways(LogLevelCritical, "InputHandler::convertSpannableStringToString Cannot allocate memory for string.");
        return WTF::String();
    }

    int destLength = 0;
    UErrorCode ec = U_ZERO_ERROR;
    u_strFromUTF32(dest.data(), destCapacity, &destLength, reinterpret_cast<UChar32*>(src->str), src->length, &ec);
    if (ec) {
        logAlways(LogLevelCritical, "InputHandler::convertSpannableStringToString Error converting string ec (%d).", ec);
        return WTF::String();
    }
    dest.resize(destLength);
    return WTF::String(dest.data(), destLength);
}

static bool getCompositionString(HIMC hInputContext, DWORD type, String& result)
{
    LONG compositionLength = Ime::ImmGetCompositionStringW(hInputContext, type, 0, 0);
    if (compositionLength <= 0)
        return false;
    Vector<UChar> compositionBuffer(compositionLength / 2);
    compositionLength = Ime::ImmGetCompositionStringW(hInputContext, type, compositionBuffer.data(), compositionLength);
    result = String::adopt(compositionBuffer);
    return true;
}

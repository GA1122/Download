static HFONT createFontIndirectAndGetWinName(const String& family, LOGFONT* winfont, String* winName)
{
    unsigned len = family.copyTo(winfont->lfFaceName, 0, LF_FACESIZE - 1);
    winfont->lfFaceName[len] = '\0';

    HFONT hfont = CreateFontIndirect(winfont);
    if (!hfont)
        return 0;

    HWndDC dc(0);
    HGDIOBJ oldFont = static_cast<HFONT>(SelectObject(dc, hfont));
    WCHAR name[LF_FACESIZE];
    unsigned resultLength = GetTextFace(dc, LF_FACESIZE, name);
    if (resultLength > 0)
        resultLength--;  

    SelectObject(dc, oldFont);
    *winName = String(name, resultLength);
    return hfont;
}

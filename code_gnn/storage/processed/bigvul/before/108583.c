static int CALLBACK traitsInFamilyEnumProc(CONST LOGFONT* logFont, CONST TEXTMETRIC* metrics, DWORD fontType, LPARAM lParam)
{
    TraitsInFamilyProcData* procData = reinterpret_cast<TraitsInFamilyProcData*>(lParam);

    unsigned traitsMask = 0;
    traitsMask |= logFont->lfItalic ? FontStyleItalicMask : FontStyleNormalMask;
    traitsMask |= FontVariantNormalMask;
    LONG weight = logFont->lfWeight;
    traitsMask |= weight == FW_THIN ? FontWeight100Mask :
        weight == FW_EXTRALIGHT ? FontWeight200Mask :
        weight == FW_LIGHT ? FontWeight300Mask :
        weight == FW_NORMAL ? FontWeight400Mask :
        weight == FW_MEDIUM ? FontWeight500Mask :
        weight == FW_SEMIBOLD ? FontWeight600Mask :
        weight == FW_BOLD ? FontWeight700Mask :
        weight == FW_EXTRABOLD ? FontWeight800Mask :
                                 FontWeight900Mask;
    procData->m_traitsMasks.add(traitsMask);
    return 1;
}

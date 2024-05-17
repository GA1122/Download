static LONG toGDIFontWeight(FontWeight fontWeight)
{
    static LONG gdiFontWeights[] = {
        FW_THIN,  
        FW_EXTRALIGHT,  
        FW_LIGHT,  
        FW_NORMAL,  
        FW_MEDIUM,  
        FW_SEMIBOLD,  
        FW_BOLD,  
        FW_EXTRABOLD,  
        FW_HEAVY  
    };
    return gdiFontWeights[fontWeight];
}

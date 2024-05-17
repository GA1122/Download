static StyleSheetContents* parseUASheet(const String& str)
{
    StyleSheetContents* sheet = StyleSheetContents::create(CSSParserContext(UASheetMode)).leakRef();  
    sheet->parseString(str);
    return sheet;
}

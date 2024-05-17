TextRun RenderBlockFlow::constructTextRun(RenderObject* context, const Font& font, const String& string, RenderStyle* style, TextRun::ExpansionBehavior expansion, TextRunFlags flags)
{
    bool hasStrongDirectionality;
    return constructTextRun(context, font, string, style,
        determineDirectionality(string, hasStrongDirectionality),
        expansion, flags);
}

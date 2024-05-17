TextRun RenderBlockFlow::constructTextRun(RenderObject* context, const Font& font, const String& string, RenderStyle* style, TextDirection direction, TextRun::ExpansionBehavior expansion, TextRunFlags flags)
{
    unsigned length = string.length();
    if (!length)
        return constructTextRunInternal(context, font, static_cast<const LChar*>(0), length, style, direction, expansion, flags);
    if (string.is8Bit())
        return constructTextRunInternal(context, font, string.characters8(), length, style, direction, expansion, flags);
    return constructTextRunInternal(context, font, string.characters16(), length, style, direction, expansion, flags);
}

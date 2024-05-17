TextRun RenderBlockFlow::constructTextRun(RenderObject* context, const Font& font, const LChar* characters, int length, RenderStyle* style, TextDirection direction, TextRun::ExpansionBehavior expansion)
{
    return constructTextRunInternal(context, font, characters, length, style, direction, expansion);
}

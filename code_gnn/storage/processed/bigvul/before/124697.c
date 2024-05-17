TextRun RenderBlockFlow::constructTextRun(RenderObject* context, const Font& font, const RenderText* text, unsigned offset, unsigned length, RenderStyle* style, TextDirection direction, TextRun::ExpansionBehavior expansion)
{
    ASSERT(offset + length <= text->textLength());
    if (text->is8Bit())
        return constructTextRunInternal(context, font, text->characters8() + offset, length, style, direction, expansion);
    return constructTextRunInternal(context, font, text->characters16() + offset, length, style, direction, expansion);
}

TextRun RenderBlockFlow::constructTextRun(RenderObject* context, const Font& font, const RenderText* text, unsigned offset, unsigned length, RenderStyle* style, TextRun::ExpansionBehavior expansion)
{
    ASSERT(offset + length <= text->textLength());
    TextRun run = text->is8Bit()
        ? constructTextRunInternal(context, font, text->characters8() + offset, length, style, LTR, expansion)
        : constructTextRunInternal(context, font, text->characters16() + offset, length, style, LTR, expansion);
    bool hasStrongDirectionality;
    run.setDirection(directionForRun(run, hasStrongDirectionality));
    return run;
}

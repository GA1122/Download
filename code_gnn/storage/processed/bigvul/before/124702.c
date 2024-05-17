static inline TextRun constructTextRunInternal(RenderObject* context, const Font& font, const CharacterType* characters, int length, RenderStyle* style, TextDirection direction, TextRun::ExpansionBehavior expansion, TextRunFlags flags)
{
    ASSERT(style);

    TextDirection textDirection = direction;
    bool directionalOverride = style->rtlOrdering() == VisualOrder;
    if (flags != DefaultTextRunFlags) {
        if (flags & RespectDirection)
            textDirection = style->direction();
        if (flags & RespectDirectionOverride)
            directionalOverride |= isOverride(style->unicodeBidi());
    }

    TextRun run(characters, length, 0, 0, expansion, textDirection, directionalOverride);
    if (textRunNeedsRenderingContext(font))
        run.setRenderingContext(SVGTextRunRenderingContext::create(context));

    return run;
}

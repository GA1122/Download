static inline TextRun constructTextRunInternal(RenderObject* context, const Font& font, const CharacterType* characters, int length, RenderStyle* style, TextDirection direction, TextRun::ExpansionBehavior expansion)
{
    ASSERT(style);

    TextDirection textDirection = direction;
    bool directionalOverride = style->rtlOrdering() == VisualOrder;

    TextRun run(characters, length, 0, 0, expansion, textDirection, directionalOverride);
    if (textRunNeedsRenderingContext(font))
        run.setRenderingContext(SVGTextRunRenderingContext::create(context));

    return run;
}

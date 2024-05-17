static inline LayoutUnit calculateMinimumPageHeight(const ComputedStyle& style, const RootInlineBox& lastLine)
{
    unsigned lineCount = std::max<unsigned>(style.hasAutoOrphans() ? 1 : style.orphans(), style.widows());
    const RootInlineBox* firstLine = &lastLine;
    for (unsigned i = 1; i < lineCount && firstLine->prevRootBox(); i++)
        firstLine = firstLine->prevRootBox();
    return lastLine.lineBottomWithLeading() - firstLine->lineTopWithLeading();
}

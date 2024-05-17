void Document::setStyleDependentState(RenderStyle* documentStyle)
{
    const Pagination& pagination = view()->pagination();
    if (pagination.mode != Pagination::Unpaginated) {
        Pagination::setStylesForPaginationMode(pagination.mode, documentStyle);
        documentStyle->setColumnGap(pagination.gap);
        if (renderView()->hasColumns())
            renderView()->updateColumnInfoFromStyle(documentStyle);
    }

    if (shouldDisplaySeamlesslyWithParent())
        return;

    FontBuilder fontBuilder;
    fontBuilder.initForStyleResolve(*this, documentStyle, isSVGDocument());
    RefPtr<CSSFontSelector> selector = m_styleResolver ? m_styleResolver->fontSelector() : 0;
    fontBuilder.createFontForDocument(selector, documentStyle);
}

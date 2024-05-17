void Document::inheritHtmlAndBodyElementStyles(StyleRecalcChange change)
{
    RenderView* renderView = this->renderView();

    if (!documentElement() || !frame() || !view())
        return;

    RefPtr<RenderStyle> documentElementStyle = documentElement()->renderStyle();
    if (!documentElementStyle || documentElement()->needsStyleRecalc() || change == Force)
        documentElementStyle = styleResolver()->styleForElement(documentElement());

    RefPtr<RenderStyle> bodyStyle = 0;
    if (body()) {
        bodyStyle = body()->renderStyle();
        if (!bodyStyle || body()->needsStyleRecalc() || documentElement()->needsStyleRecalc() || change == Force)
            bodyStyle = styleResolver()->styleForElement(body(), documentElementStyle.get());
    }

    WritingMode rootWritingMode = documentElementStyle->writingMode();
    bool isHorizontalWritingMode = documentElementStyle->isHorizontalWritingMode();
    TextDirection rootDirection = documentElementStyle->direction();

    if (!writingModeSetOnDocumentElement() && body()) {
        rootWritingMode = bodyStyle->writingMode();
        isHorizontalWritingMode = bodyStyle->isHorizontalWritingMode();
    }

    if (!directionSetOnDocumentElement() && body())
        rootDirection = bodyStyle->direction();

    RefPtr<RenderStyle> documentStyle = renderView->style();
    if (documentStyle->writingMode() != rootWritingMode || documentStyle->direction() != rootDirection) {
        RefPtr<RenderStyle> newStyle = RenderStyle::clone(documentStyle.get());
        newStyle->setWritingMode(rootWritingMode);
        newStyle->setDirection(rootDirection);
        renderView->setStyle(newStyle);
        setStyleDependentState(newStyle.get());
    }

    if (body()) {
        if (RenderStyle* style = body()->renderStyle()) {
            if (style->direction() != rootDirection || style->writingMode() != rootWritingMode)
                body()->setNeedsStyleRecalc();
        }
    }

    if (RenderStyle* style = documentElement()->renderStyle()) {
        if (style->direction() != rootDirection || style->writingMode() != rootWritingMode)
            documentElement()->setNeedsStyleRecalc();
    }
}

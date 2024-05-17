void Document::inheritHtmlAndBodyElementStyles(StyleRecalcChange change)
{
    ASSERT(inStyleRecalc());
    ASSERT(documentElement());

    bool didRecalcDocumentElement = false;
    RefPtr<ComputedStyle> documentElementStyle = documentElement()->mutableComputedStyle();
    if (change == Force)
        documentElement()->clearAnimationStyleChange();
    if (!documentElementStyle || documentElement()->needsStyleRecalc() || change == Force) {
        documentElementStyle = ensureStyleResolver().styleForElement(documentElement());
        didRecalcDocumentElement = true;
    }

    WritingMode rootWritingMode = documentElementStyle->writingMode();
    TextDirection rootDirection = documentElementStyle->direction();

    HTMLElement* body = this->body();
    RefPtr<ComputedStyle> bodyStyle;

    if (body) {
        bodyStyle = body->mutableComputedStyle();
        if (didRecalcDocumentElement)
            body->clearAnimationStyleChange();
        if (!bodyStyle || body->needsStyleRecalc() || didRecalcDocumentElement)
            bodyStyle = ensureStyleResolver().styleForElement(body, documentElementStyle.get());
        rootWritingMode = bodyStyle->writingMode();
        rootDirection = bodyStyle->direction();
    }

    RefPtr<ComputedStyle> overflowStyle;
    if (Element* element = viewportDefiningElement(documentElementStyle.get())) {
        if (element == body) {
            overflowStyle = bodyStyle;
        } else {
            ASSERT(element == documentElement());
            overflowStyle = documentElementStyle;
        }
    }

    if (styleEngine().usesRemUnits() && (documentElement()->needsAttach() || documentElement()->ensureComputedStyle()->fontSize() != documentElementStyle->fontSize())) {
        ensureStyleResolver().invalidateMatchedPropertiesCache();
        documentElement()->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::FontSizeChange));
    }

    EOverflow overflowX = OAUTO;
    EOverflow overflowY = OAUTO;
    float columnGap = 0;
    if (overflowStyle) {
        overflowX = overflowStyle->overflowX();
        overflowY = overflowStyle->overflowY();
        if (overflowX == OVISIBLE)
            overflowX = OAUTO;
        if (overflowY == OVISIBLE)
            overflowY = OAUTO;
        columnGap = overflowStyle->columnGap();
    }

    WebScrollBlocksOn scrollBlocksOn = documentElementStyle->scrollBlocksOn();

    RefPtr<ComputedStyle> documentStyle = layoutView()->mutableStyle();
    if (documentStyle->writingMode() != rootWritingMode
        || documentStyle->direction() != rootDirection
        || documentStyle->overflowX() != overflowX
        || documentStyle->overflowY() != overflowY
        || documentStyle->columnGap() != columnGap
        || documentStyle->scrollBlocksOn() != scrollBlocksOn) {
        RefPtr<ComputedStyle> newStyle = ComputedStyle::clone(*documentStyle);
        newStyle->setWritingMode(rootWritingMode);
        newStyle->setDirection(rootDirection);
        newStyle->setColumnGap(columnGap);
        newStyle->setOverflowX(overflowX);
        newStyle->setOverflowY(overflowY);
        newStyle->setScrollBlocksOn(scrollBlocksOn);
        layoutView()->setStyle(newStyle);
        setupFontBuilder(*newStyle);
    }

    if (body) {
        if (const ComputedStyle* style = body->computedStyle()) {
            if (style->direction() != rootDirection || style->writingMode() != rootWritingMode)
                body->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::WritingModeChange));
        }
    }

    if (const ComputedStyle* style = documentElement()->computedStyle()) {
        if (style->direction() != rootDirection || style->writingMode() != rootWritingMode)
            documentElement()->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::WritingModeChange));
    }
}

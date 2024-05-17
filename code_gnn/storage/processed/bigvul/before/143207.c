void Document::inheritHtmlAndBodyElementStyles(StyleRecalcChange change)
{
    DCHECK(inStyleRecalc());
    DCHECK(documentElement());

    bool didRecalcDocumentElement = false;
    RefPtr<ComputedStyle> documentElementStyle = documentElement()->mutableComputedStyle();
    if (change == Force)
        documentElement()->clearAnimationStyleChange();
    if (!documentElementStyle || documentElement()->needsStyleRecalc() || change == Force) {
        documentElementStyle = ensureStyleResolver().styleForElement(documentElement());
        didRecalcDocumentElement = true;
    }

    WritingMode rootWritingMode = documentElementStyle->getWritingMode();
    TextDirection rootDirection = documentElementStyle->direction();

    HTMLElement* body = this->body();
    RefPtr<ComputedStyle> bodyStyle;

    if (body) {
        bodyStyle = body->mutableComputedStyle();
        if (didRecalcDocumentElement)
            body->clearAnimationStyleChange();
        if (!bodyStyle || body->needsStyleRecalc() || didRecalcDocumentElement)
            bodyStyle = ensureStyleResolver().styleForElement(body, documentElementStyle.get());
        rootWritingMode = bodyStyle->getWritingMode();
        rootDirection = bodyStyle->direction();
    }

    const ComputedStyle* backgroundStyle = documentElementStyle.get();
    if (isHTMLHtmlElement(documentElement()) && isHTMLBodyElement(body) && !backgroundStyle->hasBackground())
        backgroundStyle = bodyStyle.get();
    Color backgroundColor = backgroundStyle->visitedDependentColor(CSSPropertyBackgroundColor);
    FillLayer backgroundLayers = backgroundStyle->backgroundLayers();
    for (auto currentLayer = &backgroundLayers; currentLayer; currentLayer = currentLayer->next()) {
        currentLayer->setClip(BorderFillBox);

        if (currentLayer->attachment() == ScrollBackgroundAttachment)
            currentLayer->setAttachment(LocalBackgroundAttachment);
    }
    EImageRendering imageRendering = backgroundStyle->imageRendering();

    const ComputedStyle* overflowStyle = nullptr;
    if (Element* element = viewportDefiningElement(documentElementStyle.get())) {
        if (element == body) {
            overflowStyle = bodyStyle.get();
        } else {
            DCHECK_EQ(element, documentElement());
            overflowStyle = documentElementStyle.get();

            if (bodyStyle && !bodyStyle->isOverflowVisible())
                UseCounter::count(*this, UseCounter::BodyScrollsInAdditionToViewport);
        }
    }

    if (styleEngine().usesRemUnits() && (documentElement()->needsAttach() || !documentElement()->computedStyle() || documentElement()->computedStyle()->fontSize() != documentElementStyle->fontSize())) {
        ensureStyleResolver().invalidateMatchedPropertiesCache();
        documentElement()->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::FontSizeChange));
    }

    EOverflow overflowX = OverflowAuto;
    EOverflow overflowY = OverflowAuto;
    float columnGap = 0;
    if (overflowStyle) {
        overflowX = overflowStyle->overflowX();
        overflowY = overflowStyle->overflowY();
        if (overflowX == OverflowVisible)
            overflowX = OverflowAuto;
        if (overflowY == OverflowVisible)
            overflowY = OverflowAuto;
        columnGap = overflowStyle->columnGap();
    }

    ScrollSnapType snapType = overflowStyle->getScrollSnapType();
    const LengthPoint& snapDestination = overflowStyle->scrollSnapDestination();

    RefPtr<ComputedStyle> documentStyle = layoutView()->mutableStyle();
    if (documentStyle->getWritingMode() != rootWritingMode
        || documentStyle->direction() != rootDirection
        || documentStyle->visitedDependentColor(CSSPropertyBackgroundColor) != backgroundColor
        || documentStyle->backgroundLayers() != backgroundLayers
        || documentStyle->imageRendering() != imageRendering
        || documentStyle->overflowX() != overflowX
        || documentStyle->overflowY() != overflowY
        || documentStyle->columnGap() != columnGap
        || documentStyle->getScrollSnapType() != snapType
        || documentStyle->scrollSnapDestination() != snapDestination) {
        RefPtr<ComputedStyle> newStyle = ComputedStyle::clone(*documentStyle);
        newStyle->setWritingMode(rootWritingMode);
        newStyle->setDirection(rootDirection);
        newStyle->setBackgroundColor(backgroundColor);
        newStyle->accessBackgroundLayers() = backgroundLayers;
        newStyle->setImageRendering(imageRendering);
        newStyle->setOverflowX(overflowX);
        newStyle->setOverflowY(overflowY);
        newStyle->setColumnGap(columnGap);
        newStyle->setScrollSnapType(snapType);
        newStyle->setScrollSnapDestination(snapDestination);
        layoutView()->setStyle(newStyle);
        setupFontBuilder(*newStyle);
    }

    if (body) {
        if (const ComputedStyle* style = body->computedStyle()) {
            if (style->direction() != rootDirection || style->getWritingMode() != rootWritingMode)
                body->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::WritingModeChange));
        }
    }

    if (const ComputedStyle* style = documentElement()->computedStyle()) {
        if (style->direction() != rootDirection || style->getWritingMode() != rootWritingMode)
            documentElement()->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::WritingModeChange));
    }
}

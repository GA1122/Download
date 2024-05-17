static bool buildNodeQuads(Node* node, Vector<FloatQuad>& quads)
{
    RenderObject* renderer = node->renderer();
    Frame* containingFrame = node->document().frame();

    if (!renderer || !containingFrame)
        return false;

    FrameView* containingView = containingFrame->view();
    FrameView* mainView = containingFrame->page()->mainFrame()->view();
    IntRect boundingBox = pixelSnappedIntRect(containingView->contentsToRootView(renderer->absoluteBoundingBoxRect()));
    boundingBox.move(mainView->scrollOffset());

    if (renderer->node() && renderer->node()->isSVGElement() && !renderer->isSVGRoot()) {
        renderer->absoluteQuads(quads);
        for (size_t i = 0; i < quads.size(); ++i)
            contentsQuadToPage(mainView, containingView, quads[i]);
        return false;
    }

    if (!renderer->isBox() && !renderer->isRenderInline())
        return false;

    LayoutRect contentBox;
    LayoutRect paddingBox;
    LayoutRect borderBox;
    LayoutRect marginBox;

    if (renderer->isBox()) {
        RenderBox* renderBox = toRenderBox(renderer);

        contentBox = renderBox->contentBoxRect();
        contentBox.setWidth(contentBox.width() + renderBox->verticalScrollbarWidth());
        contentBox.setHeight(contentBox.height() + renderBox->horizontalScrollbarHeight());

        paddingBox = LayoutRect(contentBox.x() - renderBox->paddingLeft(), contentBox.y() - renderBox->paddingTop(),
            contentBox.width() + renderBox->paddingLeft() + renderBox->paddingRight(), contentBox.height() + renderBox->paddingTop() + renderBox->paddingBottom());
        borderBox = LayoutRect(paddingBox.x() - renderBox->borderLeft(), paddingBox.y() - renderBox->borderTop(),
            paddingBox.width() + renderBox->borderLeft() + renderBox->borderRight(), paddingBox.height() + renderBox->borderTop() + renderBox->borderBottom());
        marginBox = LayoutRect(borderBox.x() - renderBox->marginLeft(), borderBox.y() - renderBox->marginTop(),
            borderBox.width() + renderBox->marginWidth(), borderBox.height() + renderBox->marginHeight());
    } else {
        RenderInline* renderInline = toRenderInline(renderer);

        borderBox = renderInline->linesBoundingBox();
        paddingBox = LayoutRect(borderBox.x() + renderInline->borderLeft(), borderBox.y() + renderInline->borderTop(),
            borderBox.width() - renderInline->borderLeft() - renderInline->borderRight(), borderBox.height() - renderInline->borderTop() - renderInline->borderBottom());
        contentBox = LayoutRect(paddingBox.x() + renderInline->paddingLeft(), paddingBox.y() + renderInline->paddingTop(),
            paddingBox.width() - renderInline->paddingLeft() - renderInline->paddingRight(), paddingBox.height() - renderInline->paddingTop() - renderInline->paddingBottom());
        marginBox = LayoutRect(borderBox.x() - renderInline->marginLeft(), borderBox.y(),
            borderBox.width() + renderInline->marginWidth(), borderBox.height());
    }

    FloatQuad absContentQuad = renderer->localToAbsoluteQuad(FloatRect(contentBox));
    FloatQuad absPaddingQuad = renderer->localToAbsoluteQuad(FloatRect(paddingBox));
    FloatQuad absBorderQuad = renderer->localToAbsoluteQuad(FloatRect(borderBox));
    FloatQuad absMarginQuad = renderer->localToAbsoluteQuad(FloatRect(marginBox));

    contentsQuadToPage(mainView, containingView, absContentQuad);
    contentsQuadToPage(mainView, containingView, absPaddingQuad);
    contentsQuadToPage(mainView, containingView, absBorderQuad);
    contentsQuadToPage(mainView, containingView, absMarginQuad);

    quads.append(absMarginQuad);
    quads.append(absBorderQuad);
    quads.append(absPaddingQuad);
    quads.append(absContentQuad);

    return true;
}

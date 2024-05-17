IntRect WebPagePrivate::rectForNode(Node* node)
{
    if (!node)
        return IntRect();

    RenderObject* renderer = node->renderer();

    if (!renderer)
        return IntRect();

    IntRect blockRect;

    if (m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled && renderer->isText()) {
        RenderBlock* renderBlock = renderer->containingBlock();
        int xOffset = 0;
        int yOffset = 0;
        while (!renderBlock->isRoot()) {
            xOffset += renderBlock->x();
            yOffset += renderBlock->y();
            renderBlock = renderBlock->containingBlock();
        }
        const RenderText* renderText = toRenderText(renderer);
        IntRect linesBox = renderText->linesBoundingBox();
        blockRect = IntRect(xOffset + linesBox.x(), yOffset + linesBox.y(), linesBox.width(), linesBox.height());
    } else
        blockRect = renderer->absoluteClippedOverflowRect();

    if (renderer->isText()) {
        RenderBlock* rb = renderer->containingBlock();

        int blockWidth = 0;
        int lineCount = rb->lineCount();
        for (int i = 0; i < lineCount; i++)
            blockWidth = max(blockWidth, rb->availableLogicalWidthForLine(i, false));

        blockRect.setWidth(blockWidth);
        blockRect.setX(blockRect.x() + rb->logicalLeftOffsetForLine(1, false));
    }

    if (renderer->style()->hasPadding()) {
        blockRect.setX(blockRect.x() + renderer->style()->paddingLeft().value());
        blockRect.setY(blockRect.y() + renderer->style()->paddingTop().value());
        blockRect.setWidth(blockRect.width() - renderer->style()->paddingRight().value());
        blockRect.setHeight(blockRect.height() - renderer->style()->paddingBottom().value());
    }

    return blockRect;
}

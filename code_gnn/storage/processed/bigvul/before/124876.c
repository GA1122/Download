static float getMaxWidthListMarker(const RenderBox* renderer)
{
#ifndef NDEBUG
    ASSERT(renderer);
    Node* parentNode = renderer->generatingNode();
    ASSERT(parentNode);
    ASSERT(isHTMLOListElement(parentNode) || isHTMLUListElement(parentNode));
    ASSERT(renderer->style()->textAutosizingMultiplier() != 1);
#endif
    float maxWidth = 0;
    for (RenderObject* child = renderer->firstChild(); child; child = child->nextSibling()) {
        if (!child->isListItem())
            continue;

        RenderBox* listItem = toRenderBox(child);
        for (RenderObject* itemChild = listItem->firstChild(); itemChild; itemChild = itemChild->nextSibling()) {
            if (!itemChild->isListMarker())
                continue;
            RenderBox* itemMarker = toRenderBox(itemChild);
            if (itemMarker->needsLayout())
                itemMarker->layout();
            maxWidth = max<float>(maxWidth, toRenderListMarker(itemMarker)->logicalWidth().toFloat());
            break;
        }
    }
    return maxWidth;
}

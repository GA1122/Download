void RenderFlexibleBox::prepareOrderIteratorAndMargins()
{
    OrderIteratorPopulator populator(m_orderIterator);

    for (RenderBox* child = firstChildBox(); child; child = child->nextSiblingBox()) {
        populator.collectChild(child);

        if (child->isOutOfFlowPositioned())
            continue;

        if (isHorizontalFlow()) {
            child->setMarginLeft(computeChildMarginValue(child->style()->marginLeft()));
            child->setMarginRight(computeChildMarginValue(child->style()->marginRight()));
        } else {
            child->setMarginTop(computeChildMarginValue(child->style()->marginTop()));
            child->setMarginBottom(computeChildMarginValue(child->style()->marginBottom()));
        }
    }
}

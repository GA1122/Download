void LayoutBlockFlow::addLowestFloatFromChildren(LayoutBlockFlow* block)
{
    if (!block || !block->containsFloats() || block->createsNewFormattingContext())
        return;

    FloatingObject* floatingObject = block->m_floatingObjects->lowestFloatingObject();
    if (!floatingObject || containsFloat(floatingObject->layoutObject()))
        return;

    LayoutSize offset(-block->logicalLeft(), -block->logicalTop());
    if (!isHorizontalWritingMode())
        offset = offset.transposedSize();

    if (!m_floatingObjects)
        createFloatingObjects();
    FloatingObject* newFloatingObject = m_floatingObjects->add(floatingObject->copyToNewContainer(offset, FloatingObject::IndirectlyContained));
    newFloatingObject->setIsLowestNonOverhangingFloatInChild(true);
}

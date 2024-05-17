void LayoutBlockFlow::addOverhangingFloats(LayoutBlockFlow* child, bool makeChildPaintOtherFloats)
{
    if (!child->containsFloats() || child->createsNewFormattingContext())
        return;

    LayoutUnit childLogicalTop = child->logicalTop();
    LayoutUnit childLogicalLeft = child->logicalLeft();

    FloatingObjectSetIterator childEnd = child->m_floatingObjects->set().end();
    for (FloatingObjectSetIterator childIt = child->m_floatingObjects->set().begin(); childIt != childEnd; ++childIt) {
        FloatingObject& floatingObject = *childIt->get();
        LayoutUnit logicalBottomForFloat = std::min(this->logicalBottomForFloat(floatingObject), LayoutUnit::max() - childLogicalTop);
        LayoutUnit logicalBottom = childLogicalTop + logicalBottomForFloat;

        if (logicalBottom > logicalHeight()) {
            if (!containsFloat(floatingObject.layoutObject())) {
                LayoutSize offset = isHorizontalWritingMode() ? LayoutSize(-childLogicalLeft, -childLogicalTop) : LayoutSize(-childLogicalTop, -childLogicalLeft);
                if (!m_floatingObjects)
                    createFloatingObjects();
                m_floatingObjects->add(floatingObject.copyToNewContainer(offset, FloatingObject::IndirectlyContained));
            }
        }
    }
}

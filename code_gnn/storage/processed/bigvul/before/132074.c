FloatingObject* LayoutBlockFlow::insertFloatingObject(LayoutBox& floatBox)
{
    ASSERT(floatBox.isFloating());

    if (!m_floatingObjects) {
        createFloatingObjects();
    } else {
        const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator it = floatingObjectSet.find<FloatingObjectHashTranslator>(&floatBox);
        if (it != floatingObjectSet.end())
            return it->get();
    }


    OwnPtr<FloatingObject> newObj = FloatingObject::create(&floatBox);

    bool isChildLayoutBlock = floatBox.isLayoutBlock();
    if (isChildLayoutBlock && !floatBox.needsLayout() && view()->layoutState()->pageLogicalHeightChanged())
        floatBox.setChildNeedsLayout(MarkOnlyThis);

    floatBox.layoutIfNeeded();

    setLogicalWidthForFloat(*newObj, logicalWidthForChild(floatBox) + marginStartForChild(floatBox) + marginEndForChild(floatBox));

    return m_floatingObjects->add(newObj.release());
}

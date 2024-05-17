FloatingObject* RenderBlockFlow::insertFloatingObject(RenderBox* floatBox)
{
    ASSERT(floatBox->isFloating());

    if (!m_floatingObjects) {
        createFloatingObjects();
    } else {
        const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator it = floatingObjectSet.find<FloatingObjectHashTranslator>(floatBox);
        if (it != floatingObjectSet.end())
            return *it;
    }


    OwnPtr<FloatingObject> newObj = FloatingObject::create(floatBox);

    bool isChildRenderBlock = floatBox->isRenderBlock();
    if (isChildRenderBlock && !floatBox->needsLayout() && view()->layoutState()->pageLogicalHeightChanged())
        floatBox->setChildNeedsLayout(MarkOnlyThis);

    bool needsBlockDirectionLocationSetBeforeLayout = isChildRenderBlock && view()->layoutState()->needsBlockDirectionLocationSetBeforeLayout();
    if (!needsBlockDirectionLocationSetBeforeLayout || isWritingModeRoot()) {  
        floatBox->layoutIfNeeded();
    } else {
        floatBox->updateLogicalWidth();
        floatBox->computeAndSetBlockDirectionMargins(this);
    }

    setLogicalWidthForFloat(newObj.get(), logicalWidthForChild(floatBox) + marginStartForChild(floatBox) + marginEndForChild(floatBox));

    return m_floatingObjects->add(newObj.release());
}

void RenderBox::updateFromStyle()
{
    RenderBoxModelObject::updateFromStyle();

    RenderStyle* styleToUse = style();
    bool isRootObject = isDocumentElement();
    bool isViewObject = isRenderView();

    if (isRootObject || isViewObject)
        setHasBoxDecorations(true);

    setFloating(!isOutOfFlowPositioned() && styleToUse->isFloating());

    bool boxHasOverflowClip = false;
    if (!styleToUse->isOverflowVisible() && isRenderBlock() && !isViewObject) {
        if (node() != document().viewportDefiningElement()) {
            boxHasOverflowClip = true;
            if (!hasOverflowClip()) {
                if (!RuntimeEnabledFeatures::repaintAfterLayoutEnabled())
                    repaint();
            }
        }
    }

    if (RuntimeEnabledFeatures::repaintAfterLayoutEnabled() && (boxHasOverflowClip != hasOverflowClip())) {
        for (RenderObject* child = firstChild(); child; child = child->nextSibling())
            child->setShouldDoFullRepaintIfSelfPaintingLayer(true);
    }

    setHasOverflowClip(boxHasOverflowClip);

    setHasTransform(styleToUse->hasTransformRelatedProperty());
    setHasReflection(styleToUse->boxReflect());
}

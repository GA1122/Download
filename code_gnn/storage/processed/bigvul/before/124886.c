static bool isCandidateForOpaquenessTest(RenderBox* childBox)
{
    RenderStyle* childStyle = childBox->style();
    if (childStyle->position() != StaticPosition && childBox->containingBlock() != childBox->parent())
        return false;
    if (childStyle->visibility() != VISIBLE || childStyle->shapeOutside())
        return false;
    if (!childBox->width() || !childBox->height())
        return false;
    if (RenderLayer* childLayer = childBox->layer()) {
        if (childLayer->compositingState() != NotComposited)
            return false;
        if (!childStyle->hasAutoZIndex())
            return false;
        if (childLayer->hasTransform() || childLayer->isTransparent() || childLayer->hasFilter())
            return false;
        if (childBox->hasOverflowClip() && childStyle->hasBorderRadius())
            return false;
    }
    return true;
}

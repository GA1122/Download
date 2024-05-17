void RenderBox::willBeDestroyed()
{
    clearOverrideSize();
    clearContainingBlockOverrideSize();

    RenderBlock::removePercentHeightDescendantIfNeeded(this);

    ShapeOutsideInfo::removeInfo(*this);

    RenderBoxModelObject::willBeDestroyed();
}

void RenderBox::mapAbsoluteToLocalPoint(MapCoordinatesFlags mode, TransformState& transformState) const
{
    ASSERT(!view() || !view()->layoutStateEnabled());

    bool isFixedPos = style()->position() == FixedPosition;
    bool hasTransform = hasLayer() && layer()->transform();
    if (hasTransform && !isFixedPos) {
        mode &= ~IsFixed;
    } else if (isFixedPos)
        mode |= IsFixed;

    RenderBoxModelObject::mapAbsoluteToLocalPoint(mode, transformState);
}

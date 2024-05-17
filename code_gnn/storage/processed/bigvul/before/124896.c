void RenderBox::mapLocalToContainer(const RenderLayerModelObject* repaintContainer, TransformState& transformState, MapCoordinatesFlags mode, bool* wasFixed) const
{
    if (repaintContainer == this)
        return;

    if (RenderView* v = view()) {
        if (v->canUseLayoutStateForContainer(repaintContainer)) {
            LayoutState* layoutState = v->layoutState();
            LayoutSize offset = layoutState->paintOffset() + locationOffset();
            if (style()->hasInFlowPosition() && layer())
                offset += layer()->offsetForInFlowPosition();
            transformState.move(offset);
            return;
        }
    }

    bool containerSkipped;
    RenderObject* o = container(repaintContainer, &containerSkipped);
    if (!o)
        return;

    bool isFixedPos = style()->position() == FixedPosition;
    bool hasTransform = hasLayer() && layer()->transform();
    if (hasTransform && !isFixedPos)
        mode &= ~IsFixed;
    else if (isFixedPos)
        mode |= IsFixed;

    if (wasFixed)
        *wasFixed = mode & IsFixed;

    LayoutSize containerOffset = offsetFromContainer(o, roundedLayoutPoint(transformState.mappedPoint()));

    bool preserve3D = mode & UseTransforms && (o->style()->preserves3D() || style()->preserves3D());
    if (mode & UseTransforms && shouldUseTransformFromContainer(o)) {
        TransformationMatrix t;
        getTransformFromContainer(o, containerOffset, t);
        transformState.applyTransform(t, preserve3D ? TransformState::AccumulateTransform : TransformState::FlattenTransform);
    } else
        transformState.move(containerOffset.width(), containerOffset.height(), preserve3D ? TransformState::AccumulateTransform : TransformState::FlattenTransform);

    if (containerSkipped) {
        LayoutSize containerOffset = repaintContainer->offsetFromAncestorContainer(o);
        transformState.move(-containerOffset.width(), -containerOffset.height(), preserve3D ? TransformState::AccumulateTransform : TransformState::FlattenTransform);
        return;
    }

    mode &= ~ApplyContainerFlip;

    o->mapLocalToContainer(repaintContainer, transformState, mode, wasFixed);
}

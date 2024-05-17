void SVGLayoutSupport::mapLocalToAncestor(const LayoutObject* object, const LayoutBoxModelObject* ancestor, TransformState& transformState)
{
    transformState.applyTransform(object->localToSVGParentTransform());

    LayoutObject* parent = object->parent();

    if (parent->isSVGRoot())
        transformState.applyTransform(toLayoutSVGRoot(parent)->localToBorderBoxTransform());

    MapCoordinatesFlags mode = UseTransforms;
    parent->mapLocalToAncestor(ancestor, transformState, mode);
}

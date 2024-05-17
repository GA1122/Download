const LayoutObject* SVGLayoutSupport::pushMappingToContainer(const LayoutObject* object, const LayoutBoxModelObject* ancestorToStopAt, LayoutGeometryMap& geometryMap)
{
    ASSERT_UNUSED(ancestorToStopAt, ancestorToStopAt != object);

    LayoutObject* parent = object->parent();

    if (parent->isSVGRoot()) {
        TransformationMatrix matrix(object->localToSVGParentTransform());
        matrix.multiply(toLayoutSVGRoot(parent)->localToBorderBoxTransform());
        geometryMap.push(object, matrix);
    } else {
        geometryMap.push(object, object->localToSVGParentTransform());
    }

    return parent;
}

void SVGLayoutSupport::computeContainerBoundingBoxes(const LayoutObject* container, FloatRect& objectBoundingBox, bool& objectBoundingBoxValid, FloatRect& strokeBoundingBox, FloatRect& paintInvalidationBoundingBox)
{
    objectBoundingBox = FloatRect();
    objectBoundingBoxValid = false;
    strokeBoundingBox = FloatRect();

    for (LayoutObject* current = container->slowFirstChild(); current; current = current->nextSibling()) {
        if (current->isSVGHiddenContainer())
            continue;

        if (current->isSVGShape() && toLayoutSVGShape(current)->isShapeEmpty())
            continue;

        if (current->isSVGText() && !toLayoutSVGText(current)->isObjectBoundingBoxValid())
            continue;

        const AffineTransform& transform = current->localToSVGParentTransform();
        updateObjectBoundingBox(objectBoundingBox, objectBoundingBoxValid, current,
            transform.mapRect(current->objectBoundingBox()));
        strokeBoundingBox.unite(transform.mapRect(current->paintInvalidationRectInLocalSVGCoordinates()));
    }

    paintInvalidationBoundingBox = strokeBoundingBox;
}

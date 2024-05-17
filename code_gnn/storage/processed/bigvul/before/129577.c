bool LayoutSVGContainer::nodeAtFloatPoint(HitTestResult& result, const FloatPoint& pointInParent, HitTestAction hitTestAction)
{
    if (!pointIsInsideViewportClip(pointInParent))
        return false;

    FloatPoint localPoint;
    if (!SVGLayoutSupport::transformToUserSpaceAndCheckClipping(this, localToSVGParentTransform(), pointInParent, localPoint))
        return false;

    for (LayoutObject* child = lastChild(); child; child = child->previousSibling()) {
        if (child->nodeAtFloatPoint(result, localPoint, hitTestAction)) {
            const LayoutPoint& localLayoutPoint = roundedLayoutPoint(localPoint);
            updateHitTestResult(result, localLayoutPoint);
            if (result.addNodeToListBasedTestResult(child->node(), localLayoutPoint) == StopHitTesting)
                return true;
        }
    }

    if (style()->pointerEvents() == PE_BOUNDINGBOX) {
        ASSERT(isObjectBoundingBoxValid());
        if (objectBoundingBox().contains(localPoint)) {
            const LayoutPoint& localLayoutPoint = roundedLayoutPoint(localPoint);
            updateHitTestResult(result, localLayoutPoint);
            if (result.addNodeToListBasedTestResult(element(), localLayoutPoint) == StopHitTesting)
                return true;
        }
    }
    return false;
}

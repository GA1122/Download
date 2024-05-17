bool SVGLayoutSupport::transformToUserSpaceAndCheckClipping(const LayoutObject* object, const AffineTransform& localTransform, const FloatPoint& pointInParent, FloatPoint& localPoint)
{
    if (!localTransform.isInvertible())
        return false;
    localPoint = localTransform.inverse().mapPoint(pointInParent);
    return pointInClippingArea(object, localPoint);
}

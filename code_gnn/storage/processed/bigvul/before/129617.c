static inline float distanceToChildLayoutObject(LayoutObject* child, const FloatPoint& point)
{
    const AffineTransform& localToParentTransform = child->localToSVGParentTransform();
    if (!localToParentTransform.isInvertible())
        return std::numeric_limits<float>::max();
    FloatPoint childLocalPoint = localToParentTransform.inverse().mapPoint(point);
    return child->objectBoundingBox().squaredDistanceTo(childLocalPoint);
}

bool WebPagePrivate::transformedPointEqualsUntransformedPoint(const IntPoint& transformedPoint, const IntPoint& untransformedPoint)
{
    if (m_transformationMatrix->a() > 1.0)
        return transformedPoint == mapToTransformed(untransformedPoint);

    return mapFromTransformed(transformedPoint) == untransformedPoint;
}

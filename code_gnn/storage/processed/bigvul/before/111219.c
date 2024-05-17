IntPoint WebPagePrivate::mapToTransformed(const IntPoint& point) const
{
    return roundTransformedPoint(m_transformationMatrix->mapPoint(FloatPoint(point)));
}

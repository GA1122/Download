FloatPoint WebPagePrivate::mapToTransformedFloatPoint(const FloatPoint& point) const
{
    return m_transformationMatrix->mapPoint(point);
}

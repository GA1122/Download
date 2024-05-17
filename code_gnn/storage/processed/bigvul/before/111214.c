FloatPoint WebPagePrivate::mapFromTransformedFloatPoint(const FloatPoint& point) const
{
    return m_transformationMatrix->inverse().mapPoint(point);
}

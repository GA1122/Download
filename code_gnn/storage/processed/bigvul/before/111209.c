IntPoint WebPagePrivate::mapFromTransformed(const IntPoint& point) const
{
    return roundUntransformedPoint(m_transformationMatrix->inverse().mapPoint(FloatPoint(point)));
}

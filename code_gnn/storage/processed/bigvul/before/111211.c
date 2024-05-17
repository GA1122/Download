IntRect WebPagePrivate::mapFromTransformed(const IntRect& rect) const
{
    return enclosingIntRect(m_transformationMatrix->inverse().mapRect(FloatRect(rect)));
}

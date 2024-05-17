IntRect WebPagePrivate::mapToTransformed(const IntRect& rect) const
{
    return enclosingIntRect(m_transformationMatrix->mapRect(FloatRect(rect)));
}

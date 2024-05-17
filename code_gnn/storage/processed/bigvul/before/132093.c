LayoutUnit LayoutBlockFlow::lowestFloatLogicalBottom(FloatingObject::Type floatType) const
{
    if (!m_floatingObjects)
        return LayoutUnit();

    return m_floatingObjects->lowestFloatLogicalBottom(floatType);
}

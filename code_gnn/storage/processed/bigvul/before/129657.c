FloatRect AffineTransform::mapRect(const FloatRect& rect) const
{
    if (isIdentityOrTranslation()) {
        if (!m_transform[4] && !m_transform[5])
            return rect;

        FloatRect mappedRect(rect);
        mappedRect.move(narrowPrecisionToFloat(m_transform[4]), narrowPrecisionToFloat(m_transform[5]));
        return mappedRect;
    }

    FloatQuad result;
    result.setP1(mapPoint(rect.location()));
    result.setP2(mapPoint(FloatPoint(rect.maxX(), rect.y())));
    result.setP3(mapPoint(FloatPoint(rect.maxX(), rect.maxY())));
    result.setP4(mapPoint(FloatPoint(rect.x(), rect.maxY())));
    return result.boundingBox();
}

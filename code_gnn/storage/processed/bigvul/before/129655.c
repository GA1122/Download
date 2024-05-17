FloatQuad AffineTransform::mapQuad(const FloatQuad& q) const
{
    if (isIdentityOrTranslation()) {
        FloatQuad mappedQuad(q);
        mappedQuad.move(narrowPrecisionToFloat(m_transform[4]), narrowPrecisionToFloat(m_transform[5]));
        return mappedQuad;
    }

    FloatQuad result;
    result.setP1(mapPoint(q.p1()));
    result.setP2(mapPoint(q.p2()));
    result.setP3(mapPoint(q.p3()));
    result.setP4(mapPoint(q.p4()));
    return result;
}

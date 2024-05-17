inline SVGFEColorMatrixElement::SVGFEColorMatrixElement(Document& document)
    : SVGFilterPrimitiveStandardAttributes(SVGNames::feColorMatrixTag, document)
    , m_values(SVGAnimatedNumberList::create(this, SVGNames::valuesAttr, SVGNumberList::create()))
    , m_in1(SVGAnimatedString::create(this, SVGNames::inAttr, SVGString::create()))
    , m_type(SVGAnimatedEnumeration<ColorMatrixType>::create(this, SVGNames::typeAttr, FECOLORMATRIX_TYPE_MATRIX))
{
    addToPropertyMap(m_values);
    addToPropertyMap(m_in1);
    addToPropertyMap(m_type);
}

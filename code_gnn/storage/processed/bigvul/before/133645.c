SVGAnimateElement::SVGAnimateElement(const QualifiedName& tagName, Document& document)
    : SVGAnimationElement(tagName, document)
    , m_animatedPropertyType(AnimatedString)
{
    ASSERT(hasTagName(SVGNames::animateTag) || hasTagName(SVGNames::setTag) || hasTagName(SVGNames::animateColorTag) || hasTagName(SVGNames::animateTransformTag));
    ScriptWrappable::init(this);
}

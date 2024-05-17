SVGAnimateElement::~SVGAnimateElement()
{
    if (targetElement())
        clearAnimatedType(targetElement());
}

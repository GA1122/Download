void LayoutSVGViewportContainer::calcViewport()
{
    SVGElement* element = this->element();
    ASSERT(element);
    if (!isSVGSVGElement(*element))
        return;
    SVGSVGElement* svg = toSVGSVGElement(element);
    FloatRect oldViewport = m_viewport;

    SVGLengthContext lengthContext(element);
    m_viewport = FloatRect(svg->x()->currentValue()->value(lengthContext), svg->y()->currentValue()->value(lengthContext), svg->width()->currentValue()->value(lengthContext), svg->height()->currentValue()->value(lengthContext));

    if (oldViewport != m_viewport) {
        setNeedsBoundariesUpdate();
        setNeedsTransformUpdate();
     }
 }

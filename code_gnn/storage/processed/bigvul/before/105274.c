bool HTMLElement::supportsFocus() const
{
    return Element::supportsFocus() || (rendererIsEditable() && parentNode() && !parentNode()->rendererIsEditable());
}

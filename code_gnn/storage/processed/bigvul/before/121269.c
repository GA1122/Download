void HTMLInputElement::setWidth(unsigned width)
{
    setAttribute(widthAttr, String::number(width));
}

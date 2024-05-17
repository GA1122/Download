void HTMLInputElement::setHeight(unsigned height)
{
    setAttribute(heightAttr, String::number(height));
}

void HTMLSelectElement::setSize(int size)
{
    setAttribute(sizeAttr, String::number(size));
}

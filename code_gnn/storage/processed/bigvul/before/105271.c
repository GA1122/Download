void HTMLElement::setTabIndex(int value)
{
    setAttribute(tabindexAttr, String::number(value));
}

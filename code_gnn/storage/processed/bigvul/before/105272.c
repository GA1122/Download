void HTMLElement::setTranslate(bool enable)
{
    setAttribute(translateAttr, enable ? "yes" : "no");
}

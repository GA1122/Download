void HTMLElement::setItemValue(const String& value, ExceptionCode& ec)
{
    if (!hasAttribute(itempropAttr) || hasAttribute(itemscopeAttr)) {
        ec = INVALID_ACCESS_ERR;
        return;
    }

    setItemValueText(value, ec);
}

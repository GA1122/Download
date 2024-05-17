PassRefPtr<MicroDataItemValue> HTMLElement::itemValue() const
{
    if (!hasAttribute(itempropAttr))
        return 0;

    if (hasAttribute(itemscopeAttr))
        return MicroDataItemValue::createFromNode(const_cast<HTMLElement* const>(this));

    return MicroDataItemValue::createFromString(itemValueText());
}

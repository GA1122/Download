void HTMLSelectElement::parseMultipleAttribute(const AtomicString& value)
{
    bool oldUsesMenuList = usesMenuList();
    m_multiple = !value.isNull();
    setNeedsValidityCheck();
    if (oldUsesMenuList != usesMenuList())
        lazyReattachIfAttached();
}

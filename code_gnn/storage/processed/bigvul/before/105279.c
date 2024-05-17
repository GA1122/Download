TranslateAttributeMode HTMLElement::translateAttributeMode() const
{
    const AtomicString& value = getAttribute(translateAttr);

    if (value == nullAtom)
        return TranslateAttributeInherit;
    if (equalIgnoringCase(value, "yes") || equalIgnoringCase(value, ""))
        return TranslateAttributeYes;
    if (equalIgnoringCase(value, "no"))
        return TranslateAttributeNo;

    return TranslateAttributeInherit;
}

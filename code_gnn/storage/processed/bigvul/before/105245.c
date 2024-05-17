bool HTMLElement::hasDirectionAuto() const
{
    const AtomicString& direction = fastGetAttribute(dirAttr);
    return (hasTagName(bdiTag) && direction == nullAtom) || equalIgnoringCase(direction, "auto");
}

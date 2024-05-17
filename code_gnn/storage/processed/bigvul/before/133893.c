const AtomicString& FormAssociatedElement::name() const
{
    const AtomicString& name = toHTMLElement(this)->getNameAttribute();
    return name.isNull() ? emptyAtom : name;
}

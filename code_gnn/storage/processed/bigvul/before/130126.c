bool HTMLBodyElement::hasLegalLinkAttribute(const QualifiedName& name) const
{
    return name == backgroundAttr || HTMLElement::hasLegalLinkAttribute(name);
}

bool ImageInputType::hasLegalLinkAttribute(const QualifiedName& name) const
{
    return name == srcAttr || BaseButtonInputType::hasLegalLinkAttribute(name);
}

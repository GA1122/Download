static const QualifiedName& frameOwnerURLAttributeName(const HTMLFrameOwnerElement& frameOwner)
{
    return frameOwner.hasTagName(HTMLNames::objectTag) ? HTMLNames::dataAttr : HTMLNames::srcAttr;
}

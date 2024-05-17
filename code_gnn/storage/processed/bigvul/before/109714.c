static bool isAttributeOnAllOwners(const WebCore::QualifiedName& attribute, const WebCore::QualifiedName& prefixedAttribute, const HTMLFrameOwnerElement* owner)
{
    if (!owner)
        return true;
    do {
        if (!(owner->hasAttribute(attribute) || owner->hasAttribute(prefixedAttribute)))
            return false;
    } while ((owner = owner->document().ownerElement()));
    return true;
}

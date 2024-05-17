bool Document::webkitFullscreenEnabled() const
{

    return isAttributeOnAllOwners(allowfullscreenAttr, webkitallowfullscreenAttr, ownerElement());
}

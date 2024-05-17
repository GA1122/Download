LayoutPart* Frame::ownerLayoutObject() const
{
    if (!deprecatedLocalOwner())
        return nullptr;
    LayoutObject* object = deprecatedLocalOwner()->layoutObject();
    if (!object)
        return nullptr;
    if (!object->isLayoutPart())
        return nullptr;
    return toLayoutPart(object);
}

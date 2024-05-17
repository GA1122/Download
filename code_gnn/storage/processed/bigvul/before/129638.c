bool SVGLayoutSupport::willIsolateBlendingDescendantsForObject(const LayoutObject* object)
{
    if (object->isSVGHiddenContainer())
        return false;
    if (!object->isSVGRoot() && !object->isSVGContainer())
        return false;
    return willIsolateBlendingDescendantsForStyle(object->styleRef());
}

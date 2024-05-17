bool Document::isInInvisibleSubframe() const
{
    if (!ownerElement())
        return false;  

    ASSERT(frame());
    return !frame()->ownerLayoutObject();
}

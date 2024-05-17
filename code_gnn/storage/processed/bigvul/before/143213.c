bool Document::isInInvisibleSubframe() const
{
    if (!localOwner())
        return false;  

    DCHECK(frame());
    return !frame()->ownerLayoutObject();
}

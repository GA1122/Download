HTMLFrameOwnerElement* Document::localOwner() const
{
    if (!frame())
        return 0;
    return frame()->deprecatedLocalOwner();
}

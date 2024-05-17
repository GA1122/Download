bool Document::hidden() const
{
    return pageVisibilityState() != PageVisibilityStateVisible;
}

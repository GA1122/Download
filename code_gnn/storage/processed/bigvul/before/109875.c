bool Document::webkitHidden() const
{
    return visibilityState() != PageVisibilityStateVisible;
}

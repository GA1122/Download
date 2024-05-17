bool RenderBox::canBeProgramaticallyScrolled() const
{
    Node* node = this->node();
    if (node && node->isDocumentNode())
        return true;

    if (!hasOverflowClip())
        return false;

    bool hasScrollableOverflow = hasScrollableOverflowX() || hasScrollableOverflowY();
    if (scrollsOverflow() && hasScrollableOverflow)
        return true;

    return node && node->rendererIsEditable();
}

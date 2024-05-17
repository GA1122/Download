bool RenderBlock::hasLineIfEmpty() const
{
    if (!node())
        return false;

    if (node()->isRootEditableElement())
        return true;

    if (node()->isShadowRoot() && isHTMLInputElement(*toShadowRoot(node())->host()))
        return true;

    return false;
}

bool HTMLSelectElement::childShouldCreateRenderer(const NodeRenderingContext& childContext) const
{
    if (!HTMLFormControlElementWithState::childShouldCreateRenderer(childContext))
        return false;
    if (!usesMenuList())
        return childContext.node()->hasTagName(HTMLNames::optionTag) || isHTMLOptGroupElement(childContext.node());
    return false;
}

void ContainerNode::recalcChildStyle(StyleRecalcChange change)
{
    ASSERT(document().inStyleRecalc());
    ASSERT(change >= UpdatePseudoElements || childNeedsStyleRecalc());
    ASSERT(!needsStyleRecalc());

    if (change < Force && hasRareData() && childNeedsStyleRecalc())
        checkForChildrenAdjacentRuleChanges();

    StyleResolver& styleResolver = document().ensureStyleResolver();
    Text* lastTextNode = nullptr;
    for (Node* child = lastChild(); child; child = child->previousSibling()) {
        if (child->isTextNode()) {
            toText(child)->recalcTextStyle(change, lastTextNode);
            lastTextNode = toText(child);
        } else if (child->isElementNode()) {
            Element* element = toElement(child);
            if (element->shouldCallRecalcStyle(change))
                element->recalcStyle(change, lastTextNode);
            else if (element->supportsStyleSharing())
                styleResolver.addToStyleSharingList(*element);
            if (element->layoutObject())
                lastTextNode = nullptr;
        }
    }
}

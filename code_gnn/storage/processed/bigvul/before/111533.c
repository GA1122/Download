bool InputHandler::willOpenPopupForNode(Node* node)
{
    if (!node)
        return false;

    ASSERT(!node->isInShadowTree());

    if (node->hasTagName(HTMLNames::selectTag) || node->hasTagName(HTMLNames::optionTag)) {
        return true;
    }

    if (node->isElementNode()) {
        Element* element = static_cast<Element*>(node);
        if (DOMSupport::isPopupInputField(element))
            return true;
    }

    return false;
}

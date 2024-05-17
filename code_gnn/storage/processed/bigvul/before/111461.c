bool InputHandler::didNodeOpenPopup(Node* node)
{
    if (!node)
        return false;

    ASSERT(!node->isInShadowTree());

    if (node->hasTagName(HTMLNames::selectTag))
        return openSelectPopup(static_cast<HTMLSelectElement*>(node));

    if (node->hasTagName(HTMLNames::optionTag)) {
        HTMLOptionElement* optionElement = static_cast<HTMLOptionElement*>(node);
        return openSelectPopup(optionElement->ownerSelectElement());
    }

    if (HTMLInputElement* element = node->toInputElement()) {
        if (DOMSupport::isDateTimeInputField(element))
            return openDatePopup(element, elementType(element));

        if (DOMSupport::isColorInputField(element))
            return openColorPopup(element);
    }
    return false;
}

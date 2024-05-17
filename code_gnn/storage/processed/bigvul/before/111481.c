static VirtualKeyboardEnterKeyType keyboardEnterKeyTypeAttribute(const WebCore::Element* element)
{
    DEFINE_STATIC_LOCAL(QualifiedName, keyboardEnterKeyTypeAttr, (nullAtom, "data-blackberry-virtual-keyboard-enter-key", nullAtom));

    if (element->fastHasAttribute(keyboardEnterKeyTypeAttr)) {
        AtomicString attributeString = element->fastGetAttribute(keyboardEnterKeyTypeAttr);
        return convertStringToKeyboardEnterKeyType(attributeString);
    }

    if (element->isFormControlElement()) {
        const HTMLFormControlElement* formElement = static_cast<const HTMLFormControlElement*>(element);
        if (formElement->form() && formElement->form()->fastHasAttribute(keyboardEnterKeyTypeAttr)) {
            AtomicString attributeString = formElement->form()->fastGetAttribute(keyboardEnterKeyTypeAttr);
            return convertStringToKeyboardEnterKeyType(attributeString);
        }
    }

    return VKBEnterKeyNotSet;
}

BlackBerryInputType InputHandler::elementType(Element* element) const
{
    if (const HTMLInputElement* inputElement = static_cast<const HTMLInputElement*>(element->toInputElement()))
        return convertInputType(inputElement);

    if (element->hasTagName(HTMLNames::textareaTag))
        return InputTypeTextArea;

    return InputTypeTextArea;
}

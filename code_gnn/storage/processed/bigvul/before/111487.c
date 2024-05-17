bool InputHandler::openDatePopup(HTMLInputElement* element, BlackBerryInputType type)
{
    if (!element || element->isDisabledOrReadOnly() || !DOMSupport::isDateTimeInputField(element))
        return false;

    if (isActiveTextEdit())
        clearCurrentFocusElement();

    switch (type) {
    case BlackBerry::Platform::InputTypeDate:
    case BlackBerry::Platform::InputTypeTime:
    case BlackBerry::Platform::InputTypeDateTime:
    case BlackBerry::Platform::InputTypeDateTimeLocal:
    case BlackBerry::Platform::InputTypeMonth: {
        element->document()->frame()->selection()->setCaretVisible(false);

        m_webPage->m_page->chrome()->client()->closePagePopup(0);
        WTF::String value = element->value();
        WTF::String min = element->getAttribute(HTMLNames::minAttr).string();
        WTF::String max = element->getAttribute(HTMLNames::maxAttr).string();
        double step = element->getAttribute(HTMLNames::stepAttr).toDouble();

        DatePickerClient* client = new DatePickerClient(type, value, min, max, step,  m_webPage, element);
        return m_webPage->m_page->chrome()->client()->openPagePopup(client,  WebCore::IntRect());
        }
    default:  
        return false;
    }
}

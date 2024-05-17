void BaseMultipleFieldsDateAndTimeInputType::handleKeydownEvent(KeyboardEvent* event)
{
    Document* document = element()->document();
    RefPtr<RenderTheme> theme = document->page() ? document->page()->theme() : RenderTheme::defaultTheme();
    if (m_pickerIndicatorIsVisible
        && ((event->keyIdentifier() == "Down" && event->getModifierState("Alt")) || (theme->shouldOpenPickerWithF4Key() && event->keyIdentifier() == "F4"))) {
        if (m_pickerIndicatorElement)
            m_pickerIndicatorElement->openPopup();
        event->setDefaultHandled();
    } else
        forwardEvent(event);
}

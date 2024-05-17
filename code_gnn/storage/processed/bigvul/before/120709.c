void BaseMultipleFieldsDateAndTimeInputType::createShadowSubtree()
{
    ASSERT(element()->shadow());

    ASSERT(!element()->attached());

    Document* document = element()->document();
    ContainerNode* container = element()->userAgentShadowRoot();

    container->appendChild(DateTimeEditElement::create(document, *this));
    updateInnerTextValue();
    container->appendChild(ClearButtonElement::create(document, *this));
    container->appendChild(SpinButtonElement::create(document, *this));

    bool shouldAddPickerIndicator = false;
    if (InputType::themeSupportsDataListUI(this))
        shouldAddPickerIndicator = true;
    RefPtr<RenderTheme> theme = document->page() ? document->page()->theme() : RenderTheme::defaultTheme();
    if (theme->supportsCalendarPicker(formControlType())) {
        shouldAddPickerIndicator = true;
        m_pickerIndicatorIsAlwaysVisible = true;
    }
    if (shouldAddPickerIndicator) {
        container->appendChild(PickerIndicatorElement::create(document, *this));
        m_pickerIndicatorIsVisible = true;
        updatePickerIndicatorVisibility();
    }
}

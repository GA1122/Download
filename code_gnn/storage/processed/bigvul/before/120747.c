void BaseMultipleFieldsDateAndTimeInputType::updatePickerIndicatorVisibility()
{
    if (m_pickerIndicatorIsAlwaysVisible) {
        showPickerIndicator();
        return;
    }
    if (RuntimeEnabledFeatures::dataListElementEnabled()) {
        if (HTMLDataListElement* dataList = element()->dataList()) {
            RefPtr<HTMLCollection> options = dataList->options();
            for (unsigned i = 0; HTMLOptionElement* option = toHTMLOptionElement(options->item(i)); ++i) {
                if (element()->isValidValue(option->value())) {
                    showPickerIndicator();
                    return;
                }
            }
        }
        hidePickerIndicator();
    }
}

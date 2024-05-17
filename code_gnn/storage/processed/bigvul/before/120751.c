BaseMultipleFieldsDateAndTimeInputType::~BaseMultipleFieldsDateAndTimeInputType()
{
    if (SpinButtonElement* element = spinButtonElement())
        element->removeSpinButtonOwner();
    if (ClearButtonElement* element = clearButtonElement())
        element->removeClearButtonOwner();
    if (DateTimeEditElement* element = dateTimeEditElement())
        element->removeEditControlOwner();
    if (PickerIndicatorElement* element = pickerIndicatorElement())
        element->removePickerIndicatorOwner();
}

void BaseMultipleFieldsDateAndTimeInputType::clearValue()
{
    RefPtr<HTMLInputElement> input(element());
    input->setValue("", DispatchInputAndChangeEvent);
    input->updateClearButtonVisibility();
}

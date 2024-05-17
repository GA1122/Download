void BaseMultipleFieldsDateAndTimeInputType::editControlValueChanged()
{
    RefPtr<HTMLInputElement> input(element());
    String oldValue = input->value();
    String newValue = sanitizeValue(dateTimeEditElement()->value());
    if ((oldValue.isEmpty() && newValue.isEmpty()) || oldValue == newValue)
        input->setNeedsValidityCheck();
    else {
        input->setValueInternal(newValue, DispatchNoEvent);
        input->setNeedsStyleRecalc();
        input->dispatchFormControlInputEvent();
        input->dispatchFormControlChangeEvent();
    }
    input->notifyFormStateChanged();
    input->updateClearButtonVisibility();
}

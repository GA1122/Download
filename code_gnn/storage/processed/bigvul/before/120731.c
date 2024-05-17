void BaseMultipleFieldsDateAndTimeInputType::restoreFormControlState(const FormControlState& state)
{
    DateTimeEditElement* edit = dateTimeEditElement();
    if (!edit)
        return;
    DateTimeFieldsState dateTimeFieldsState = DateTimeFieldsState::restoreFormControlState(state);
    edit->setValueAsDateTimeFieldsState(dateTimeFieldsState);
    element()->setValueInternal(sanitizeValue(edit->value()), DispatchNoEvent);
    updateClearButtonVisibility();
}

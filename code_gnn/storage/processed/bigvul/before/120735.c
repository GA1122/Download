bool BaseMultipleFieldsDateAndTimeInputType::shouldClearButtonRespondToMouseEvents()
{
    return !element()->isDisabledOrReadOnly() && !element()->isRequired();
}

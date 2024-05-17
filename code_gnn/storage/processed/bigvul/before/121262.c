void HTMLInputElement::setValue(const String& value, ExceptionCode& ec, TextFieldEventBehavior eventBehavior)
{
    if (isFileUpload() && !value.isEmpty()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    setValue(value, eventBehavior);
}

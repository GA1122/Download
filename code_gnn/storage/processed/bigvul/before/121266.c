void HTMLInputElement::setValueForUser(const String& value)
{
    setValue(value, DispatchChangeEvent);
}

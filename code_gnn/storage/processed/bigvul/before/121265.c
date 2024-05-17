void HTMLInputElement::setValueAsNumber(double newValue, ExceptionCode& ec, TextFieldEventBehavior eventBehavior)
{
    if (!std::isfinite(newValue)) {
        ec = NOT_SUPPORTED_ERR;
        return;
    }
    m_inputType->setValueAsDouble(newValue, eventBehavior, ec);
}

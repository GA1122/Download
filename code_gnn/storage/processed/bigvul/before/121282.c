void HTMLInputElement::stepDown(int n, ExceptionCode& ec)
{
    m_inputType->stepUp(-n, ec);
}

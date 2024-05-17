String XSLTProcessor::getParameter(const String&  , const String& localName) const
{
    return m_parameters.get(localName);
}

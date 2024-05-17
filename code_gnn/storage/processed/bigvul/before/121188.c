bool HTMLInputElement::isSpeechEnabled() const
{
    return m_inputType->shouldRespectSpeechAttribute() && RuntimeEnabledFeatures::speechInputEnabled() && hasAttribute(webkitspeechAttr);
}

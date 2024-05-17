bool InputHandler::shouldSpellCheckElement(const Element* element) const
{
    DOMSupport::AttributeState spellCheckAttr = DOMSupport::elementSupportsSpellCheck(element);

    if (spellCheckAttr == DOMSupport::Off)
        return false;

    if (spellCheckAttr == DOMSupport::Default && (m_currentFocusElementTextEditMask & NO_AUTO_TEXT))
        return false;

    return true;
}

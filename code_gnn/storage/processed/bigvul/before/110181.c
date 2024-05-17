void DateTimeFieldElement::initialize(const AtomicString& pseudo, const String& axHelpText, int axMinimum, int axMaximum)
{
    setAttribute(aria_helpAttr, axHelpText);
    setAttribute(aria_valueminAttr, String::number(axMinimum));
    setAttribute(aria_valuemaxAttr, String::number(axMaximum));
    setPseudo(pseudo);
    appendChild(Text::create(document(), visibleValue()));
}

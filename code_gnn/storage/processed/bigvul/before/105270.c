void HTMLElement::setSpellcheck(bool enable)
{
    setAttribute(spellcheckAttr, enable ? "true" : "false");
}

void HTMLSelectElement::remove(HTMLOptionElement* option)
{
    if (option->ownerSelectElement() != this)
        return;

    option->remove(IGNORE_EXCEPTION);
}

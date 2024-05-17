ValidationMessageClient* HTMLFormControlElement::validationMessageClient() const
{
    Page* page = document().page();
    if (!page)
        return nullptr;

    return &page->validationMessageClient();
}

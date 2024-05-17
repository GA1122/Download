void HTMLFormControlElement::attach(const AttachContext& context)
{
    HTMLElement::attach(context);

    if (!layoutObject())
        return;

    layoutObject()->updateFromElement();

    if (shouldAutofocusOnAttach(this))
        document().setAutofocusElement(this);
}

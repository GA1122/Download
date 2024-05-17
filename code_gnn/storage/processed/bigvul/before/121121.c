void HTMLInputElement::attach()
{
    PostAttachCallbackDisabler disabler(this);

    if (!m_hasType)
        updateType();

    HTMLTextFormControlElement::attach();

    m_inputType->attach();

    if (document()->focusedNode() == this)
        document()->updateFocusAppearanceSoon(true  );
}

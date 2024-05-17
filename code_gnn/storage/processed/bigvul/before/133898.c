void FormAssociatedElement::resetFormAttributeTargetObserver()
{
    ASSERT(toHTMLElement(this)->inDocument());
    m_formAttributeTargetObserver = FormAttributeTargetObserver::create(toHTMLElement(this)->fastGetAttribute(formAttr), this);
}

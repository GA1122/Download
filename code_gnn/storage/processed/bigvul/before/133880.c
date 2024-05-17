PassOwnPtr<FormAttributeTargetObserver> FormAttributeTargetObserver::create(const AtomicString& id, FormAssociatedElement* element)
{
    return adoptPtr(new FormAttributeTargetObserver(id, element));
}

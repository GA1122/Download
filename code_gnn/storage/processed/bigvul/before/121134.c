PassOwnPtr<ListAttributeTargetObserver> ListAttributeTargetObserver::create(const AtomicString& id, HTMLInputElement* element)
{
    return adoptPtr(new ListAttributeTargetObserver(id, element));
}

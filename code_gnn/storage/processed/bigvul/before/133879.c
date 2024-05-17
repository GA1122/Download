FormAttributeTargetObserver::FormAttributeTargetObserver(const AtomicString& id, FormAssociatedElement* element)
    : IdTargetObserver(toHTMLElement(element)->treeScope().idTargetObserverRegistry(), id)
    , m_element(element)
{
}

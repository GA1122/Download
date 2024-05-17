void DateTimeSymbolicFieldElement::setEmptyValue(EventBehavior eventBehavior)
{
    if (isReadOnly())
        return;
    m_selectedIndex = invalidIndex;
    updateVisibleValue(eventBehavior);
}

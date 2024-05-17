void RuleFeatureSet::collectInvalidationSetsForClass(InvalidationLists& invalidationLists, Element& element, const AtomicString& className) const
{
    if (RefPtrWillBeRawPtr<InvalidationData> invalidationData = m_classInvalidationSets.get(className)) {
        if (invalidationData->descendants()) {
            TRACE_SCHEDULE_STYLE_INVALIDATION(element, *invalidationData->descendants(), classChange, className);
            invalidationLists.descendants.append(invalidationData->descendants());
        }
        if (invalidationData->siblings()) {
            if (element.parentElement())
                TRACE_SCHEDULE_STYLE_INVALIDATION(*element.parentElement(), *invalidationData->siblings(), classChange, className);
            invalidationLists.siblings.append(invalidationData->siblings());
        }
    }
}

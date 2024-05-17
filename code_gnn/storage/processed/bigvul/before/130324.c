void RuleFeatureSet::collectInvalidationSetsForId(InvalidationLists& invalidationLists, Element& element, const AtomicString& id) const
{
    if (RefPtrWillBeRawPtr<InvalidationData> invalidationData = m_idInvalidationSets.get(id)) {
        if (invalidationData->descendants()) {
            TRACE_SCHEDULE_STYLE_INVALIDATION(element, *invalidationData->descendants(), idChange, id);
            invalidationLists.descendants.append(invalidationData->descendants());
        }
        if (invalidationData->siblings()) {
            if (element.parentElement())
                TRACE_SCHEDULE_STYLE_INVALIDATION(*element.parentElement(), *invalidationData->siblings(), idChange, id);
            invalidationLists.siblings.append(invalidationData->siblings());
        }
    }
}

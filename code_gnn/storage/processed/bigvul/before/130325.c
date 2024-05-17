void RuleFeatureSet::collectInvalidationSetsForPseudoClass(InvalidationLists& invalidationLists, Element& element, CSSSelector::PseudoType pseudo) const
{
    if (RefPtrWillBeRawPtr<InvalidationData> invalidationData = m_pseudoInvalidationSets.get(pseudo)) {
        if (invalidationData->descendants()) {
            TRACE_SCHEDULE_STYLE_INVALIDATION(element, *invalidationData->descendants(), pseudoChange, pseudo);
            invalidationLists.descendants.append(invalidationData->descendants());
        }
        if (invalidationData->siblings()) {
            if (element.parentElement())
                TRACE_SCHEDULE_STYLE_INVALIDATION(*element.parentElement(), *invalidationData->siblings(), pseudoChange, pseudo);
            invalidationLists.siblings.append(invalidationData->siblings());
        }
    }
}

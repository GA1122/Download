void RuleFeatureSet::addFeaturesToInvalidationSets(const CSSSelector* selector, InvalidationSetFeatures* siblingFeatures, InvalidationSetFeatures& descendantFeatures)
{
    const CSSSelector* lastCompoundSelectorInAdjacentChain = selector;

    InvalidationSetFeatures localFeatures;

    for (const CSSSelector* current = selector; current; current = current->tagHistory()) {
        InvalidationType type = siblingFeatures ? InvalidateSiblings : InvalidateDescendants;
        if (InvalidationSet* invalidationSet = invalidationSetForSelector(*current, type)) {
            if (siblingFeatures) {
                SiblingInvalidationSet* siblingInvalidationSet = toSiblingInvalidationSet(invalidationSet);
                siblingInvalidationSet->updateMaxDirectAdjacentSelectors(siblingFeatures->maxDirectAdjacentSelectors);

                addFeaturesToInvalidationSet(*invalidationSet, *siblingFeatures);
                if (siblingFeatures == &descendantFeatures)
                    siblingInvalidationSet->descendants().setInvalidatesSelf();
                else
                    addFeaturesToInvalidationSet(siblingInvalidationSet->descendants(), descendantFeatures);
            } else {
                addFeaturesToInvalidationSet(*invalidationSet, descendantFeatures);
            }
        } else {
            if (current->isHostPseudoClass())
                descendantFeatures.treeBoundaryCrossing = true;
            if (current->isInsertionPointCrossing())
                descendantFeatures.insertionPointCrossing = true;
            if (const CSSSelectorList* selectorList = current->selectorList()) {
                ASSERT(supportsInvalidationWithSelectorList(current->pseudoType()));
                for (const CSSSelector* subSelector = selectorList->first(); subSelector; subSelector = CSSSelectorList::next(*subSelector))
                    addFeaturesToInvalidationSets(subSelector, siblingFeatures, descendantFeatures);
            }
        }

        if (current->relation() == CSSSelector::SubSelector)
            continue;

        if (current->isShadowSelector())
            descendantFeatures.treeBoundaryCrossing = true;

        if (!current->isAdjacentSelector()) {
            lastCompoundSelectorInAdjacentChain = current->tagHistory();
            siblingFeatures = nullptr;
            continue;
        }

        if (siblingFeatures) {
            if (siblingFeatures->maxDirectAdjacentSelectors == UINT_MAX)
                continue;

            if (current->relation() == CSSSelector::DirectAdjacent)
                siblingFeatures->maxDirectAdjacentSelectors++;
            else
                siblingFeatures->maxDirectAdjacentSelectors = UINT_MAX;
            continue;
        }

        localFeatures = InvalidationSetFeatures();
        auto result = extractInvalidationSetFeatures(*lastCompoundSelectorInAdjacentChain, localFeatures, Ancestor);
        ASSERT(result.first);
        localFeatures.forceSubtree = result.second == ForceSubtree;
        siblingFeatures = &localFeatures;
    }
}

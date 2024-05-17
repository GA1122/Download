void ContainerNode::checkForSiblingStyleChanges(SiblingCheckType changeType, Node* nodeBeforeChange, Node* nodeAfterChange)
{
    if (!inActiveDocument() || document().hasPendingForcedStyleRecalc() || styleChangeType() >= SubtreeStyleChange)
        return;

    if (((childrenAffectedByForwardPositionalRules() || childrenAffectedByIndirectAdjacentRules()) && nodeAfterChange)
        || (childrenAffectedByBackwardPositionalRules() && nodeBeforeChange)) {
        setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
        return;
    }

    if (childrenAffectedByFirstChildRules() && nodeAfterChange) {
        ASSERT(changeType != FinishedParsingChildren);
        Element* firstChildElement = ElementTraversal::firstChild(*this);

        Element* elementAfterChange = nodeAfterChange->isElementNode() ? toElement(nodeAfterChange) : ElementTraversal::nextSibling(*nodeAfterChange);

        if (changeType == SiblingElementInserted && elementAfterChange && firstChildElement != elementAfterChange
            && (!nodeBeforeChange || !nodeBeforeChange->isElementNode()) && elementAfterChange->affectedByFirstChildRules()) {
            elementAfterChange->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
        }

        if (changeType == SiblingElementRemoved && firstChildElement == elementAfterChange && firstChildElement && firstChildElement->affectedByFirstChildRules())
            firstChildElement->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
    }

    if (childrenAffectedByLastChildRules() && nodeBeforeChange) {
        Element* lastChildElement = ElementTraversal::lastChild(*this);

        Element* elementBeforeChange = nodeBeforeChange->isElementNode() ? toElement(nodeBeforeChange) : ElementTraversal::previousSibling(*nodeBeforeChange);

        if (changeType == SiblingElementInserted && elementBeforeChange && lastChildElement != elementBeforeChange
            && (!nodeAfterChange || !nodeAfterChange->isElementNode()) && elementBeforeChange->affectedByLastChildRules()) {
            elementBeforeChange->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
        }

        if ((changeType == SiblingElementRemoved || changeType == FinishedParsingChildren) && lastChildElement == elementBeforeChange && lastChildElement && lastChildElement->affectedByLastChildRules())
            lastChildElement->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
    }

    if (childrenAffectedByDirectAdjacentRules() && nodeAfterChange) {
        if (Element* elementAfterChange = nodeAfterChange->isElementNode() ? toElement(nodeAfterChange) : ElementTraversal::nextSibling(*nodeAfterChange))
            elementAfterChange->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
    }
}

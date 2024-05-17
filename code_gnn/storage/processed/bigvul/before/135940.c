void ContainerNode::focusStateChanged()
{
    if (!layoutObject())
        return;

    if (styleChangeType() < SubtreeStyleChange) {
        if (computedStyle()->affectedByFocus() && computedStyle()->hasPseudoStyle(FIRST_LETTER))
            setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::createWithExtraData(StyleChangeReason::PseudoClass, StyleChangeExtraData::Focus));
        else if (isElementNode() && toElement(this)->childrenOrSiblingsAffectedByFocus())
            document().styleEngine().pseudoStateChangedForElement(CSSSelector::PseudoFocus, *toElement(this));
        else if (computedStyle()->affectedByFocus())
            setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::createWithExtraData(StyleChangeReason::PseudoClass, StyleChangeExtraData::Focus));
    }

    LayoutTheme::theme().controlStateChanged(*layoutObject(), FocusControlState);
}

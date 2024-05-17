void ContainerNode::setActive(bool down)
{
    if (down == active())
        return;

    Node::setActive(down);

    if (layoutObject()) {
        if (styleChangeType() < SubtreeStyleChange) {
            if (computedStyle()->affectedByActive() && computedStyle()->hasPseudoStyle(FIRST_LETTER))
                setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::createWithExtraData(StyleChangeReason::PseudoClass, StyleChangeExtraData::Active));
            else if (isElementNode() && toElement(this)->childrenOrSiblingsAffectedByActive())
                document().styleEngine().pseudoStateChangedForElement(CSSSelector::PseudoActive, *toElement(this));
            else if (computedStyle()->affectedByActive())
                setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::createWithExtraData(StyleChangeReason::PseudoClass, StyleChangeExtraData::Active));
        }

        LayoutTheme::theme().controlStateChanged(*layoutObject(), PressedControlState);
    }
}

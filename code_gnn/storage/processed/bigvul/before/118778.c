void ContainerNode::setActive(bool down, bool pause)
{
    if (down == active()) return;

    Node::setActive(down);

    if (renderer()) {
        if (renderStyle()->affectedByActive() || (isElementNode() && toElement(this)->childrenAffectedByActive()))
            setNeedsStyleRecalc();
        if (renderStyle()->hasAppearance())
            RenderTheme::theme().stateChanged(renderer(), PressedState);
    }
}

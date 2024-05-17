void HTMLAnchorElement::setActive(bool down)
{
    if (hasEditableStyle())
        return;

    ContainerNode::setActive(down);
}

Element* ContainerNode::getElementById(const AtomicString& id) const
{
    if (isInTreeScope()) {
        Element* element = treeScope().getElementById(id);
        if (!element)
            return nullptr;
        if (element->isDescendantOf(this))
            return element;
    }

    for (Element& element : ElementTraversal::descendantsOf(*this)) {
        if (element.getIdAttribute() == id)
            return &element;
    }
    return nullptr;
}

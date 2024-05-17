Element* Document::pointerLockElement() const
{
    if (!page() || page()->pointerLockController().lockPending())
        return 0;
    if (Element* element = page()->pointerLockController().element()) {
        if (element->document() == this)
            return element;
    }
    return 0;
}

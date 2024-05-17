void Document::webkitExitPointerLock()
{
    if (!page())
        return;
    if (Element* target = page()->pointerLockController().element()) {
        if (target->document() != this)
            return;
    }
    page()->pointerLockController().requestPointerUnlock();
}

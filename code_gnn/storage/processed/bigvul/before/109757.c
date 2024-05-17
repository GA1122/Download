void Document::prepareForDestruction()
{
    disconnectDescendantFrames();

    if (!confusingAndOftenMisusedAttached())
        return;

    if (DOMWindow* window = this->domWindow())
        window->willDetachDocumentFromFrame();
    detach();
}

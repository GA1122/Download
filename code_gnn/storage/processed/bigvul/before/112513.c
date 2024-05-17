void Document::prepareForDestruction()
{
    disconnectDescendantFrames();
    if (DOMWindow* window = this->domWindow())
        window->willDetachDocumentFromFrame();
    detach();
}

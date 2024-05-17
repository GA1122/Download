static bool skipBodyBackground(const RenderBox* bodyElementRenderer)
{
    ASSERT(bodyElementRenderer->isBody());
    RenderObject* documentElementRenderer = bodyElementRenderer->document().documentElement()->renderer();
    return documentElementRenderer
        && !documentElementRenderer->hasBackground()
        && (documentElementRenderer == bodyElementRenderer->parent());
}

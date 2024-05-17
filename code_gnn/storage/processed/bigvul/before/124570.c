static inline bool isEditingBoundary(RenderObject* ancestor, RenderObject* child)
{
    ASSERT(!ancestor || ancestor->nonPseudoNode());
    ASSERT(child && child->nonPseudoNode());
    return !ancestor || !ancestor->parent() || (ancestor->hasLayer() && ancestor->parent()->isRenderView())
        || ancestor->nonPseudoNode()->rendererIsEditable() == child->nonPseudoNode()->rendererIsEditable();
}

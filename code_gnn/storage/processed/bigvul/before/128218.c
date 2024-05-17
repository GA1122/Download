void FrameView::addWidgetToUpdate(RenderEmbeddedObject& object)
{
    ASSERT(isInPerformLayout());
    Node* node = object.node();
    ASSERT(node);
    if (isHTMLObjectElement(*node) || isHTMLEmbedElement(*node))
        toHTMLPlugInElement(node)->setNeedsWidgetUpdate(true);

    m_widgetUpdateSet.add(&object);
}

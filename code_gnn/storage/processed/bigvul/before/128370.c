bool FrameView::updateWidgets()
{
    ASSERT(!m_widgetUpdateSet.isEmpty());
    if (m_nestedLayoutCount > 1 || m_widgetUpdateSet.isEmpty())
        return true;

    EmbeddedObjectSet objects;
    objects.swap(m_widgetUpdateSet);

    for (EmbeddedObjectSet::iterator it = objects.begin(); it != objects.end(); ++it) {
        RenderEmbeddedObject& object = **it;
        HTMLPlugInElement* element = toHTMLPlugInElement(object.node());

        if (!element)
            continue;

        if (object.showsUnavailablePluginIndicator())
            continue;

        if (element->needsWidgetUpdate())
            element->updateWidget();
        object.updateWidgetPosition();

        m_widgetUpdateSet.remove(&object);
    }

    return m_widgetUpdateSet.isEmpty();
}

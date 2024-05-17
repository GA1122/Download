unsigned WebInspectorProxy::platformInspectedWindowHeight()
{
    GtkAllocation allocation;
    gtk_widget_get_allocation(m_page->viewWidget(), &allocation);
    return allocation.height;
}

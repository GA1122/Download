static void inspectorViewDestroyed(GtkWidget*, gpointer userData)
{
    WebInspectorProxy* inspectorProxy = static_cast<WebInspectorProxy*>(userData);

    inspectorProxy->close();
}

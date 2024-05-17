static void contextMenuConnectActivate(GtkMenuItem* item, ContextMenuController* controller)
{
    if (GTK_IS_SEPARATOR_MENU_ITEM(item))
        return;

    if (GtkWidget* menu = gtk_menu_item_get_submenu(item)) {
        gtk_container_foreach(GTK_CONTAINER(menu), (GtkCallback)contextMenuConnectActivate, controller);
        return;
    }

    g_signal_connect(item, "activate", G_CALLBACK(contextMenuItemActivated), controller);
}

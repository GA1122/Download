static void contextMenuItemActivated(GtkMenuItem* item, ContextMenuController* controller)
{
    ContextMenuItem contextItem(item);
    controller->contextMenuItemSelected(&contextItem);
}

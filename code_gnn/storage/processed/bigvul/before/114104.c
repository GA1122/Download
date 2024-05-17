create_toolbar_button(Evas_Object *window, const char *icon_name)
{
    Evas_Object *button = elm_button_add(window);

    Evas_Object *icon = elm_icon_add(window);
    elm_icon_standard_set(icon, icon_name);
    evas_object_size_hint_max_set(icon, TOOL_BAR_ICON_SIZE, TOOL_BAR_ICON_SIZE);
    evas_object_color_set(icon, 44, 44, 102, 128);
    evas_object_show(icon);
    elm_object_part_content_set(button, "icon", icon);
    evas_object_size_hint_min_set(button, TOOL_BAR_BUTTON_SIZE, TOOL_BAR_BUTTON_SIZE);

    return button;
}

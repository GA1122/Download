void BlockedPlugin::ShowContextMenu(const WebKit::WebMouseEvent& event) {
  WebContextMenuData menu_data;
  WebVector<WebMenuItemInfo> custom_items(static_cast<size_t>(4));

  WebMenuItemInfo name_item;
  name_item.label = name_;
  custom_items[0] = name_item;

  WebMenuItemInfo separator_item;
  separator_item.type = WebMenuItemInfo::Separator;
  custom_items[1] = separator_item;

  WebMenuItemInfo run_item;
  run_item.action = kMenuActionLoad;
  run_item.enabled = true;
  run_item.label = WebString::fromUTF8(
      l10n_util::GetStringUTF8(IDS_CONTENT_CONTEXT_PLUGIN_RUN).c_str());
  custom_items[2] = run_item;

  WebMenuItemInfo hide_item;
  hide_item.action = kMenuActionRemove;
  hide_item.enabled = true;
  hide_item.label = WebString::fromUTF8(
      l10n_util::GetStringUTF8(IDS_CONTENT_CONTEXT_PLUGIN_HIDE).c_str());
  custom_items[3] = hide_item;

  menu_data.customItems.swap(custom_items);
  menu_data.mousePosition = WebPoint(event.windowX, event.windowY);
  render_view()->showContextMenu(NULL, menu_data);
  gLastActiveMenu = this;
}

void AddWindowSpecifics(int window_id,
                        const std::vector<int>& tab_list,
                        sync_pb::SessionSpecifics* meta) {
  sync_pb::SessionHeader* header = meta->mutable_header();
  sync_pb::SessionWindow* window = header->add_window();
  window->set_window_id(window_id);
  window->set_selected_tab_index(0);
  window->set_browser_type(sync_pb::SessionWindow_BrowserType_TYPE_TABBED);
  for (std::vector<int>::const_iterator iter = tab_list.begin();
       iter != tab_list.end(); ++iter) {
    window->add_tab(*iter);
  }
}

void Browser::SetAsDelegate(TabContentsWrapper* tab, Browser* delegate) {
  tab->tab_contents()->set_delegate(delegate);
  tab->set_delegate(delegate);

  tab->blocked_content_tab_helper()->set_delegate(delegate);
  tab->search_engine_tab_helper()->set_delegate(delegate);
}

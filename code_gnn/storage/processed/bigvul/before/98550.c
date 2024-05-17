void WmOverviewSnapshot::UpdateIndex(Browser* browser, int index) {
  vector<int> params;
  params.push_back(x11_util::GetX11WindowFromGtkWidget(
      GTK_WIDGET(browser->window()->GetNativeHandle())));
  params.push_back(index);
  WmIpc::instance()->SetWindowType(
      GetNativeView(),
      WM_IPC_WINDOW_CHROME_TAB_SNAPSHOT,
      &params);
  index_ = index;
}

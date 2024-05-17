app_list::AppListPresenter* ShellDelegateImpl::GetAppListPresenter() {
  if (!app_list_presenter_) {
    app_list_presenter_.reset(new app_list::AppListPresenterImpl(
        app_list_presenter_delegate_factory_.get()));
  }
  return app_list_presenter_.get();
}

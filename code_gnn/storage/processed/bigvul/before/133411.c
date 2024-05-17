ShellDelegateImpl::ShellDelegateImpl()
    : shelf_delegate_(nullptr),
      app_list_presenter_delegate_factory_(new AppListPresenterDelegateFactory(
          base::WrapUnique(new AppListViewDelegateFactoryImpl))) {}

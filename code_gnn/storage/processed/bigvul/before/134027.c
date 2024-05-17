  void CreateBuilder() {
    ResetBuilder();   

    model_.reset(new app_list::AppListModel);
    controller_.reset(new test::TestAppListControllerDelegate);
    builder_.reset(new ExtensionAppModelBuilder(controller_.get()));
    builder_->InitializeWithProfile(profile_.get(), model_.get());
  }

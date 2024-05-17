void LockContentsView::CreateHighDensityLayout(
    const std::vector<mojom::LoginUserInfoPtr>& users) {
  auto* fill = new NonAccessibleView();
  main_view_->AddChildViewAt(fill, 0);
  main_layout_->SetFlexForView(fill, 1);

  fill = new NonAccessibleView();
  main_view_->AddChildView(fill);
  main_layout_->SetFlexForView(fill, 1);

  users_list_ =
      BuildScrollableUsersListView(users, LoginDisplayStyle::kExtraSmall);
  main_view_->AddChildView(users_list_);
}

void LockContentsView::CreateMediumDensityLayout(
    const std::vector<mojom::LoginUserInfoPtr>& users) {
  main_view_->AddChildViewAt(MakeOrientationViewWithWidths(
                                 kMediumDensityMarginLeftOfAuthUserLandscapeDp,
                                 kMediumDensityMarginLeftOfAuthUserPortraitDp),
                             0);
  main_view_->AddChildView(MakeOrientationViewWithWidths(
      kMediumDensityDistanceBetweenAuthUserAndUsersLandscapeDp,
      kMediumDensityDistanceBetweenAuthUserAndUsersPortraitDp));

  users_list_ = BuildScrollableUsersListView(users, LoginDisplayStyle::kSmall);
  main_view_->AddChildView(users_list_);

  auto* left = new NonAccessibleView();
  main_view_->AddChildViewAt(left, 0);
  auto* right = new NonAccessibleView();
  main_view_->AddChildView(right);
  AddRotationAction(base::BindRepeating(
      [](views::BoxLayout* layout, views::View* left, views::View* right,
         bool landscape) {
        if (landscape) {
          layout->SetFlexForView(left, 1);
          layout->SetFlexForView(right, 1);
        } else {
          layout->SetFlexForView(left, 2);
          layout->SetFlexForView(right, 1);
        }
      },
      main_layout_, left, right));
}

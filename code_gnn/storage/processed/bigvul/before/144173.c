void LockContentsView::CreateLowDensityLayout(
    const std::vector<mojom::LoginUserInfoPtr>& users) {
  DCHECK_EQ(users.size(), 2u);

  main_view_->AddChildView(MakeOrientationViewWithWidths(
      kLowDensityDistanceBetweenUsersInLandscapeDp,
      kLowDensityDistanceBetweenUsersInPortraitDp));

  opt_secondary_big_view_ =
      AllocateLoginBigUserView(users[1], false  );
  main_view_->AddChildView(opt_secondary_big_view_);
}

ScrollableUsersListView* LockContentsView::BuildScrollableUsersListView(
    const std::vector<mojom::LoginUserInfoPtr>& users,
    LoginDisplayStyle display_style) {
  auto* view = new ScrollableUsersListView(
      users,
      base::BindRepeating(&LockContentsView::SwapToBigUser,
                          base::Unretained(this)),
      display_style);
  view->ClipHeightTo(view->contents()->size().height(), size().height());
  return view;
}

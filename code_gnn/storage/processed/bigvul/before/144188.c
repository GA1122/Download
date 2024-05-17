views::View* LockContentsView::MakeOrientationViewWithWidths(int landscape,
                                                             int portrait) {
  auto* view = new MultiSizedView(gfx::Size(landscape, kNonEmptyHeightDp),
                                  gfx::Size(portrait, kNonEmptyHeightDp));
  AddRotationAction(base::BindRepeating(&MultiSizedView::SwapPreferredSizeTo,
                                        base::Unretained(view)));
  return view;
}

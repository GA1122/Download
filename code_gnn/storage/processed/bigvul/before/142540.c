views::View* FindFirstOrLastFocusableChild(views::View* root,
                                           bool find_last_child) {
  views::FocusSearch search(root, find_last_child  ,
                            false  );
  views::FocusTraversable* dummy_focus_traversable;
  views::View* dummy_focus_traversable_view;
  return search.FindNextFocusableView(
      root,
      find_last_child ? views::FocusSearch::SearchDirection::kBackwards
                      : views::FocusSearch::SearchDirection::kForwards,
      views::FocusSearch::TraversalDirection::kDown,
      views::FocusSearch::StartingViewPolicy::kSkipStartingView,
      views::FocusSearch::AnchoredDialogPolicy::kCanGoIntoAnchoredDialog,
      &dummy_focus_traversable, &dummy_focus_traversable_view);
}

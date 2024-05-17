  void GoToOffsetWithParams(int offset,
                            const PageState& state,
                            const CommonNavigationParams common_params,
                            CommitNavigationParams commit_params) {
    EXPECT_TRUE(common_params.transition & ui::PAGE_TRANSITION_FORWARD_BACK);
    int pending_offset = offset + view()->history_list_offset_;

    commit_params.page_state = state;
    commit_params.nav_entry_id = pending_offset + 1;
    commit_params.pending_history_list_offset = pending_offset;
    commit_params.current_history_list_offset = view()->history_list_offset_;
    commit_params.current_history_list_length = view()->history_list_length_;
    frame()->Navigate(common_params, commit_params);

    FrameLoadWaiter(frame()).Wait();
  }

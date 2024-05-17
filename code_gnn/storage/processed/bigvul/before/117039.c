SessionCommand* BaseSessionService::CreateUpdateTabNavigationCommand(
    SessionID::id_type command_id,
    SessionID::id_type tab_id,
    int index,
    const NavigationEntry& entry) {
  Pickle pickle;
  pickle.WriteInt(tab_id);
  pickle.WriteInt(index);


  static const SessionCommand::size_type max_state_size =
      std::numeric_limits<SessionCommand::size_type>::max() - 1024;

  int bytes_written = 0;

  WriteStringToPickle(pickle, &bytes_written, max_state_size,
                      entry.GetVirtualURL().spec());

  WriteString16ToPickle(pickle, &bytes_written, max_state_size,
                        entry.GetTitle());

  std::string content_state_without_post;
  if (entry.GetHasPostData()) {
    content_state_without_post =
        webkit_glue::RemoveFormDataFromHistoryState(entry.GetContentState());
    int original_size = entry.GetContentState().size() / 1024;
    int new_size = content_state_without_post.size() / 1024;
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "SessionService.ContentStateSizeWithPostOriginal", original_size,
        62, 100000, 50);
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "SessionService.ContentStateSizeWithPostRemoved", new_size,
        62, 100000, 50);
  } else {
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "SessionService.ContentStateSizeWithoutPost",
        entry.GetContentState().size() / 1024,
        62, 100000, 50);
  }

  std::string content_state;

  if (!save_post_data_) {
    if (entry.GetHasPostData())
      content_state = content_state_without_post;
    else
      content_state = entry.GetContentState();
  }

  WriteStringToPickle(pickle, &bytes_written, max_state_size, content_state);

  pickle.WriteInt(entry.GetTransitionType());
  int type_mask = entry.GetHasPostData() ? TabNavigation::HAS_POST_DATA : 0;
  pickle.WriteInt(type_mask);

  WriteStringToPickle(pickle, &bytes_written, max_state_size,
      entry.GetReferrer().url.is_valid() ?
          entry.GetReferrer().url.spec() : std::string());
  pickle.WriteInt(entry.GetReferrer().policy);

  if (save_post_data_) {
    CompressDataHelper::CompressAndWriteStringToPickle(
        entry.GetContentState(), max_state_size, &pickle, &bytes_written);
  }

  return new SessionCommand(command_id, pickle);
}

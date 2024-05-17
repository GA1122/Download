bool FrameLoader::ShouldClose(bool is_reload) {
  Page* page = frame_->GetPage();
  if (!page || !page->GetChromeClient().CanOpenBeforeUnloadConfirmPanel())
    return true;

  HeapVector<Member<LocalFrame>> target_frames;
  target_frames.push_back(frame_);
  for (Frame* child = frame_->Tree().FirstChild(); child;
       child = child->Tree().TraverseNext(frame_)) {
    if (child->IsLocalFrame())
      target_frames.push_back(ToLocalFrame(child));
  }

  bool should_close = false;
  {
    NavigationDisablerForBeforeUnload navigation_disabler;
    size_t i;

    bool did_allow_navigation = false;
    for (i = 0; i < target_frames.size(); i++) {
      if (!target_frames[i]->Tree().IsDescendantOf(frame_))
        continue;
      if (!target_frames[i]->GetDocument()->DispatchBeforeUnloadEvent(
              page->GetChromeClient(), is_reload, did_allow_navigation))
        break;
    }

    if (i == target_frames.size())
      should_close = true;
  }

  return should_close;
}

bool FrameLoader::ShouldClose(bool is_reload) {
  Page* page = frame_->GetPage();
  if (!page || !page->GetChromeClient().CanOpenBeforeUnloadConfirmPanel())
    return true;

  HeapVector<Member<LocalFrame>> descendant_frames;
  for (Frame* child = frame_->Tree().FirstChild(); child;
       child = child->Tree().TraverseNext(frame_)) {
    if (auto* child_local_frame = DynamicTo<LocalFrame>(child))
      descendant_frames.push_back(child_local_frame);
  }

  {
    NavigationDisablerForBeforeUnload navigation_disabler;
    bool did_allow_navigation = false;


    IgnoreOpensDuringUnloadCountIncrementer ignore_opens_during_unload(
        frame_->GetDocument());
    if (!frame_->GetDocument()->DispatchBeforeUnloadEvent(
            &page->GetChromeClient(), is_reload, did_allow_navigation))
      return false;

    for (Member<LocalFrame>& descendant_frame : descendant_frames) {
      if (!descendant_frame->Tree().IsDescendantOf(frame_))
        continue;

      IgnoreOpensDuringUnloadCountIncrementer
          ignore_opens_during_unload_descendant(
              descendant_frame->GetDocument());
      if (!descendant_frame->GetDocument()->DispatchBeforeUnloadEvent(
              &page->GetChromeClient(), is_reload, did_allow_navigation))
        return false;
    }
  }

  return true;
}

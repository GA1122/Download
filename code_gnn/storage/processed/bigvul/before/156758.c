bool RemoteFrame::BubbleLogicalScrollFromChildFrame(
    ScrollDirection direction,
    ScrollGranularity granularity,
    Frame* child) {
  DCHECK(child->IsLocalFrame());
  DCHECK(child->Client());
  ToLocalFrame(child)->Client()->BubbleLogicalScrollInParentFrame(direction,
                                                                  granularity);
  return false;
}

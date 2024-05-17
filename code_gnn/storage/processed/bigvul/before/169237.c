RenderFrameHost* FrameMatchingPredicate(
    WebContents* web_contents,
    const base::Callback<bool(RenderFrameHost*)>& predicate) {
  std::set<RenderFrameHost*> frame_set;
  web_contents->ForEachFrame(base::BindRepeating(
      &AddToSetIfFrameMatchesPredicate, &frame_set, predicate));
  EXPECT_EQ(1U, frame_set.size());
  return frame_set.size() == 1 ? *frame_set.begin() : nullptr;
}

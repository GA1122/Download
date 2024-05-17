    RemoveObservedFrame(RenderFrameHost* render_frame_host, int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  auto iters = observed_frames_.equal_range(render_frame_host);
  auto it = std::find_if(iters.first, iters.second,
                         [render_frame_host, session_id](
                             std::pair<RenderFrameHost* const, int>& map_pair) {
                           return map_pair.first == render_frame_host &&
                                  map_pair.second == session_id;
                         });

  if (it == iters.second)
    return;

  observed_frames_.erase(it);
  if (!observed_frames_.size())
    parent_observer_->contents_observers_.erase(web_contents());

}

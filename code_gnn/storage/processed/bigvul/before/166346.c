    RenderFrameDeleted(RenderFrameHost* render_frame_host) {
  auto iters = observed_frames_.equal_range(render_frame_host);
  for (auto it = iters.first; it != iters.second; ++it) {
    base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::IO})
        ->PostTask(FROM_HERE,
                   base::BindOnce(parent_observer_->frame_deleted_callback_,
                                  it->second));
  }

  observed_frames_.erase(iters.first, iters.second);
  if (!observed_frames_.size())
    parent_observer_->contents_observers_.erase(web_contents());

}

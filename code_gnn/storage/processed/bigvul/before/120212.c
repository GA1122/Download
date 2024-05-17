void LayerTreeHost::Composite(base::TimeTicks frame_begin_time) {
  if (!proxy_->HasImplThread())
    static_cast<SingleThreadProxy*>(proxy_.get())->CompositeImmediately(
        frame_begin_time);
  else
    SetNeedsCommit();
}

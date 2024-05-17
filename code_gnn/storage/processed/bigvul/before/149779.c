void LayerTreeHost::Composite(base::TimeTicks frame_begin_time) {
  DCHECK(IsSingleThreaded());
  DCHECK(!settings_.single_thread_proxy_scheduler);
  SingleThreadProxy* proxy = static_cast<SingleThreadProxy*>(proxy_.get());

  proxy->CompositeImmediately(frame_begin_time);
}

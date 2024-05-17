void RenderFrameImpl::OnStop() {
  DCHECK(frame_);

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
  frame_->StopLoading();
  if (!weak_this)
    return;

  for (auto& observer : observers_)
    observer.OnStop();
}

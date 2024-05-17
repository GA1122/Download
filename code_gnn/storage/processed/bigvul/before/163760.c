void DelegatedFrameHost::CopyFromCompositingSurfaceToVideoFrame(
    const gfx::Rect& src_subrect,
    scoped_refptr<media::VideoFrame> target,
    const base::Callback<void(const gfx::Rect&, bool)>& callback) {
  if (!CanCopyFromCompositingSurface()) {
    callback.Run(gfx::Rect(), false);
    return;
  }

  std::unique_ptr<viz::CopyOutputRequest> request =
      viz::CopyOutputRequest::CreateRequest(base::BindOnce(
          &DelegatedFrameHost::CopyFromCompositingSurfaceHasResultForVideo,
          AsWeakPtr(),   
          nullptr, std::move(target), callback));
  if (!src_subrect.IsEmpty())
    request->set_area(src_subrect);
  RequestCopyOfOutput(std::move(request));
}

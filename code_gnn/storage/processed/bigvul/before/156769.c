void RemoteFrame::Navigate(const FrameLoadRequest& passed_request,
                           WebFrameLoadType frame_load_type) {
  FrameLoadRequest frame_request(passed_request);

  FrameLoader::SetReferrerForFrameRequest(frame_request);
  FrameLoader::UpgradeInsecureRequest(frame_request.GetResourceRequest(),
                                      frame_request.OriginDocument());

  Client()->Navigate(frame_request.GetResourceRequest(),
                     frame_load_type == WebFrameLoadType::kReplaceCurrentItem,
                     frame_request.GetBlobURLToken());
}

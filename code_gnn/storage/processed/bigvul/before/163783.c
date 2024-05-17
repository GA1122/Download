void DelegatedFrameHost::RequestCopyOfOutput(
    std::unique_ptr<viz::CopyOutputRequest> request) {
  if (!request->has_area())
    request->set_area(gfx::Rect(current_frame_size_in_dip_));

  if (request_copy_of_output_callback_for_testing_.is_null()) {
    client_->DelegatedFrameHostGetLayer()->RequestCopyOfOutput(
        std::move(request));
  } else {
    request_copy_of_output_callback_for_testing_.Run(std::move(request));
  }
}

void RenderFrameHostImpl::OnRunFileChooser(const FileChooserParams& params) {
  if (params.default_file_name != params.default_file_name.BaseName()) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_FILE_CHOOSER_PATH);
    return;
  }

  delegate_->RunFileChooser(this, params);
}

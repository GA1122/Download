  static void InstallDelegatedFrameHostClient(
      RenderWidgetHostViewAura* view,
      std::unique_ptr<DelegatedFrameHostClient> delegated_frame_host_client) {
    if (!view->frame_sink_id_.is_valid())
      return;

    view->delegated_frame_host_client_ = std::move(delegated_frame_host_client);
    view->delegated_frame_host_ = nullptr;
    view->delegated_frame_host_ = std::make_unique<DelegatedFrameHost>(
        view->frame_sink_id_, view->delegated_frame_host_client_.get(),
        false  );
  }

  void CreateVizFrameSinkManager() {
    viz::mojom::FrameSinkManagerPtr frame_sink_manager;
    viz::mojom::FrameSinkManagerRequest frame_sink_manager_request =
        mojo::MakeRequest(&frame_sink_manager);
    viz::mojom::FrameSinkManagerClientPtr frame_sink_manager_client;
    viz::mojom::FrameSinkManagerClientRequest
        frame_sink_manager_client_request =
            mojo::MakeRequest(&frame_sink_manager_client);

    host_frame_sink_manager.BindAndSetManager(
        std::move(frame_sink_manager_client_request),
        base::ThreadTaskRunnerHandle::Get(), std::move(frame_sink_manager));

    host_frame_sink_manager.SetConnectionLostCallback(base::BindRepeating(
        []() { CompositorDependencies::Get().CreateVizFrameSinkManager(); }));

    pending_connect_viz_on_io_thread_ = base::BindOnce(
        &CompositorDependencies::ConnectVizFrameSinkManagerOnIOThread,
        std::move(frame_sink_manager_request),
        frame_sink_manager_client.PassInterface());
  }

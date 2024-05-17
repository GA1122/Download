void RenderFrameHostImpl::SetRenderFrameCreated(bool created) {
  if (created && delegate_)
    CHECK(!delegate_->IsBeingDestroyed());

  bool was_created = render_frame_created_;
  render_frame_created_ = created;

  if (delegate_ && (created != was_created)) {
    if (created) {
      SetUpMojoIfNeeded();
      delegate_->RenderFrameCreated(this);
    } else {
      delegate_->RenderFrameDeleted(this);
    }
  }

  if (created && render_widget_host_) {
    mojom::WidgetPtr widget;
    GetRemoteInterfaces()->GetInterface(&widget);
    render_widget_host_->SetWidget(std::move(widget));

    if (frame_input_handler_) {
      mojom::WidgetInputHandlerAssociatedPtr widget_handler;
      mojom::WidgetInputHandlerHostPtr host;
      mojom::WidgetInputHandlerHostRequest host_request =
          mojo::MakeRequest(&host);
      frame_input_handler_->GetWidgetInputHandler(
          mojo::MakeRequest(&widget_handler), std::move(host));
      render_widget_host_->SetWidgetInputHandler(std::move(widget_handler),
                                                 std::move(host_request));
    }
    render_widget_host_->input_router()->SetFrameTreeNodeId(
        frame_tree_node_->frame_tree_node_id());
    viz::mojom::InputTargetClientPtr input_target_client;
    remote_interfaces_->GetInterface(&input_target_client);
    input_target_client_ = input_target_client.get();
    render_widget_host_->SetInputTargetClient(std::move(input_target_client));
    render_widget_host_->InitForFrame();
  }

  if (enabled_bindings_ && created) {
    if (!frame_bindings_control_)
      GetRemoteAssociatedInterfaces()->GetInterface(&frame_bindings_control_);
    frame_bindings_control_->AllowBindings(enabled_bindings_);
  }
}

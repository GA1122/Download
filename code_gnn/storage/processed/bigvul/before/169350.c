bool TestChildOrGuestAutoresize(bool is_guest,
                                RenderProcessHost* embedder_rph,
                                RenderWidgetHost* guest_rwh) {
  RenderProcessHostImpl* embedder_rph_impl =
      static_cast<RenderProcessHostImpl*>(embedder_rph);
  RenderWidgetHostImpl* guest_rwh_impl =
      static_cast<RenderWidgetHostImpl*>(guest_rwh);

  scoped_refptr<SynchronizeVisualPropertiesMessageFilter> filter(
      new SynchronizeVisualPropertiesMessageFilter());

  if (is_guest) {
    embedder_rph_impl->SetBrowserPluginMessageFilterSubFilterForTesting(
        filter.get());
  } else {
    embedder_rph_impl->AddFilter(filter.get());
  }

  viz::LocalSurfaceId current_id =
      guest_rwh_impl->GetView()->GetLocalSurfaceId();
  if (!current_id.is_valid())
    current_id = filter->WaitForSurfaceId();

  gfx::Size min_size(10, 10);
  gfx::Size max_size(100, 100);
  guest_rwh_impl->SetAutoResize(true, min_size, max_size);
  guest_rwh_impl->GetView()->EnableAutoResize(min_size, max_size);

  current_id = filter->WaitForSurfaceId();

  int routing_id = guest_rwh_impl->GetRoutingID();
  ViewHostMsg_ResizeOrRepaint_ACK_Params params;
  params.view_size = gfx::Size(75, 75);
  params.flags = 0;
  params.child_allocated_local_surface_id = viz::LocalSurfaceId(
      current_id.parent_sequence_number(),
      current_id.child_sequence_number() + 1, current_id.embed_token());
  guest_rwh_impl->OnMessageReceived(
      ViewHostMsg_ResizeOrRepaint_ACK(routing_id, params));

  base::RunLoop().RunUntilIdle();

  guest_rwh_impl->GetView()->DisableAutoResize(params.view_size);

  return filter->WaitForSurfaceId() ==
         viz::LocalSurfaceId(current_id.parent_sequence_number() + 1,
                             current_id.child_sequence_number() + 1,
                             current_id.embed_token());
}

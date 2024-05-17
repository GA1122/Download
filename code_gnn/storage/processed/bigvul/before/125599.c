void RenderViewHostImpl::EnableAutoResize(const gfx::Size& min_size,
                                          const gfx::Size& max_size) {
  SetShouldAutoResize(true);
  Send(new ViewMsg_EnableAutoResize(GetRoutingID(), min_size, max_size));
}

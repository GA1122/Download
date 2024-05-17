void RenderViewImpl::DidInitiatePaint() {
  pepper_delegate_.ViewInitiatedPaint();
  if (GetGuestToEmbedderChannel())
    GetGuestToEmbedderChannel()->IssueSwapBuffers(guest_graphics_resource());
}

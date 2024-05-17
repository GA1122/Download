  RendererURLRequestContextSelector(Profile* profile,
                                    int render_child_id)
      : request_context_(profile->GetRequestContextForRenderProcess(
                             render_child_id)),
        media_request_context_(profile->GetRequestContextForMedia()) {
  }

void Document::InitContentSecurityPolicy(
    ContentSecurityPolicy* csp,
    const ContentSecurityPolicy* last_origin_document_csp) {
  SetContentSecurityPolicy(csp ? csp : ContentSecurityPolicy::Create());

  GetContentSecurityPolicy()->BindToDelegate(
      GetContentSecurityPolicyDelegate());

  if (last_origin_document_csp &&
      (url_.IsEmpty() || url_.ProtocolIsAbout() || url_.ProtocolIsData() ||
       url_.ProtocolIs("blob") || url_.ProtocolIs("filesystem"))) {
    GetContentSecurityPolicy()->CopyStateFrom(last_origin_document_csp);
  }

  if (IsPluginDocument()) {
    if (last_origin_document_csp) {
      GetContentSecurityPolicy()->CopyPluginTypesFrom(last_origin_document_csp);
    } else if (frame_) {
      Frame* inherit_from = frame_->Tree().Parent()
                                ? frame_->Tree().Parent()
                                : frame_->Client()->Opener();
      if (inherit_from && frame_ != inherit_from) {
        DCHECK(inherit_from->GetSecurityContext() &&
               inherit_from->GetSecurityContext()->GetContentSecurityPolicy());
        GetContentSecurityPolicy()->CopyPluginTypesFrom(
            inherit_from->GetSecurityContext()->GetContentSecurityPolicy());
      }
    }
  }
}

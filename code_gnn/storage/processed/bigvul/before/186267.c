 void Document::InitContentSecurityPolicy(ContentSecurityPolicy* csp) {
  
  
  
  
// void Document::InitContentSecurityPolicy(
//     ContentSecurityPolicy* csp,
//     const ContentSecurityPolicy* policy_to_inherit) {
    SetContentSecurityPolicy(csp ? csp : ContentSecurityPolicy::Create());
  
  if (frame_) {
//   if (policy_to_inherit) {
//     GetContentSecurityPolicy()->CopyStateFrom(policy_to_inherit);
//   } else if (frame_) {
      Frame* inherit_from = frame_->Tree().Parent() ? frame_->Tree().Parent()
                                                    : frame_->Client()->Opener();
      if (inherit_from && frame_ != inherit_from) {
        DCHECK(inherit_from->GetSecurityContext() &&
               inherit_from->GetSecurityContext()->GetContentSecurityPolicy());
      ContentSecurityPolicy* policy_to_inherit =
//       policy_to_inherit =
            inherit_from->GetSecurityContext()->GetContentSecurityPolicy();
        if (url_.IsEmpty() || url_.ProtocolIsAbout() || url_.ProtocolIsData() ||
            url_.ProtocolIs("blob") || url_.ProtocolIs("filesystem")) {
          GetContentSecurityPolicy()->CopyStateFrom(policy_to_inherit);
        }
      if (IsPluginDocument())
        GetContentSecurityPolicy()->CopyPluginTypesFrom(policy_to_inherit);
      }
    }
//    
//    
//   if (policy_to_inherit && IsPluginDocument())
//     GetContentSecurityPolicy()->CopyPluginTypesFrom(policy_to_inherit);
// 
    GetContentSecurityPolicy()->BindToExecutionContext(this);
  }
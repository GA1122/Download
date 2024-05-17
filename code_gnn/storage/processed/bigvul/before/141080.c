ContentSecurityPolicy* Document::GetContentSecurityPolicyForWorld() {
  v8::Isolate* isolate = GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> v8_context = isolate->GetCurrentContext();

  if (v8_context.IsEmpty())
    return GetContentSecurityPolicy();

  DOMWrapperWorld& world = DOMWrapperWorld::Current(isolate);
  if (!world.IsIsolatedWorld())
    return GetContentSecurityPolicy();

  int world_id = world.GetWorldId();
  auto it = isolated_world_csp_map_->find(world_id);
  if (it != isolated_world_csp_map_->end())
    return it->value;

  ContentSecurityPolicy* policy =
      IsolatedWorldCSP::Get().CreateIsolatedWorldCSP(*this, world_id);
  if (!policy)
    return GetContentSecurityPolicy();

  isolated_world_csp_map_->insert(world_id, policy);
  return policy;
}

void RenderProcessHostImpl::EarlyZygoteLaunch() {
  DCHECK(!g_render_zygote);
  ZygoteHostImpl::GetInstance()->SetRendererSandboxStatus(
      (*GetGenericZygote())->GetSandboxStatus());
}

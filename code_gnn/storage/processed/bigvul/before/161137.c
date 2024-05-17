bool MediaStreamManager::IsOriginAllowed(int render_process_id,
                                         const url::Origin& origin) {
  if (!ChildProcessSecurityPolicyImpl::GetInstance()->CanRequestURL(
          render_process_id, origin.GetURL())) {
    LOG(ERROR) << "MSM: Renderer requested a URL it's not allowed to use: "
               << origin.Serialize();
    return false;
  }

  return true;
}

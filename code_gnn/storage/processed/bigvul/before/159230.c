bool CanRequestURLFromRenderer(int render_process_id, GURL url) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!ChildProcessSecurityPolicyImpl::GetInstance()->CanRequestURL(
          render_process_id, url)) {
    DVLOG(1) << "Denied unauthorized download request for "
             << url.possibly_invalid_spec();
    return false;
  }
  return true;
}

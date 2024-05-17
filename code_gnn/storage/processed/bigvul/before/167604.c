bool SiteInstanceImpl::IsDefaultSubframeSiteInstance() const {
  return process_reuse_policy_ ==
         ProcessReusePolicy::USE_DEFAULT_SUBFRAME_PROCESS;
}

bool DataReductionProxyConfig::GetIsCaptivePortal() const {
  DCHECK(thread_checker_.CalledOnValidThread());

#if defined(OS_ANDROID)
  return net::android::GetIsCaptivePortal();
#endif   
  return false;
}

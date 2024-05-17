void NavigationURLLoaderImpl::SetBeginNavigationInterceptorForTesting(
    const BeginNavigationInterceptor& interceptor) {
  DCHECK(!BrowserThread::IsThreadInitialized(BrowserThread::IO) ||
         BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
  g_interceptor.Get() = interceptor;
}

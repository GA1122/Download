void RenderFrameHostImpl::NavigateToInterstitialURL(const GURL& data_url) {
  DCHECK(data_url.SchemeIs(url::kDataScheme));
  CommonNavigationParams common_params(
      data_url, Referrer(), ui::PAGE_TRANSITION_LINK,
      FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT, false, false,
      base::TimeTicks::Now(), FrameMsg_UILoadMetricsReportType::NO_REPORT,
      GURL(), GURL(), PREVIEWS_OFF, base::TimeTicks::Now(), "GET", nullptr,
      base::Optional<SourceLocation>(),
      CSPDisposition::CHECK  );
  if (IsBrowserSideNavigationEnabled()) {
    CommitNavigation(nullptr, nullptr, mojo::ScopedDataPipeConsumerHandle(),
                     common_params, RequestNavigationParams(), false);
  } else {
    Navigate(common_params, StartNavigationParams(), RequestNavigationParams());
  }
}

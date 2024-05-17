void RenderFrameHostImpl::NavigateToInterstitialURL(const GURL& data_url) {
  TRACE_EVENT1("navigation", "RenderFrameHostImpl::NavigateToInterstitialURL",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id());
  DCHECK(data_url.SchemeIs(url::kDataScheme));
  NavigationDownloadPolicy download_policy;
  download_policy.SetDisallowed(NavigationDownloadType::kInterstitial);

  CommonNavigationParams common_params(
      data_url, base::nullopt, Referrer(), ui::PAGE_TRANSITION_LINK,
      FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT, download_policy, false,
      GURL(), GURL(), PREVIEWS_OFF, base::TimeTicks::Now(), "GET", nullptr,
      base::Optional<SourceLocation>(), false  ,
      false  , InitiatorCSPInfo(), std::string());
  CommitNavigation(nullptr  , nullptr  ,
                   network::mojom::URLLoaderClientEndpointsPtr(), common_params,
                   CommitNavigationParams(), false, base::nullopt,
                   base::nullopt  ,
                   nullptr  ,
                   base::UnguessableToken::Create()  );
}

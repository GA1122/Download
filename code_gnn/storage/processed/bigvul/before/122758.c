void BrowserPluginGuest::Observe(int type,
                                 const NotificationSource& source,
                                 const NotificationDetails& details) {
  switch (type) {
    case NOTIFICATION_RESOURCE_RECEIVED_REDIRECT: {
      DCHECK_EQ(Source<WebContents>(source).ptr(), web_contents());
      ResourceRedirectDetails* resource_redirect_details =
            Details<ResourceRedirectDetails>(details).ptr();
      bool is_top_level =
          resource_redirect_details->resource_type == ResourceType::MAIN_FRAME;
      LoadRedirect(resource_redirect_details->url,
                   resource_redirect_details->new_url,
                   is_top_level);
      break;
    }
    default:
      NOTREACHED() << "Unexpected notification sent.";
      break;
  }
}

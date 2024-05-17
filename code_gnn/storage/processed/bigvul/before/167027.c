void NavigationControllerImpl::RendererDidNavigateToExistingPage(
    RenderFrameHostImpl* rfh,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    bool is_same_document,
    bool was_restored,
    NavigationHandleImpl* handle) {
  DCHECK(GetLastCommittedEntry()) << "ClassifyNavigation should guarantee "
                                  << "that a last committed entry exists.";

  DCHECK(!rfh->GetParent());

  NavigationEntryImpl* entry;
  if (params.intended_as_new_entry) {
    entry = GetLastCommittedEntry();
    if (!is_same_document)
      entry->GetSSL() = SSLStatus(handle->GetSSLInfo());

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      bool has_cert = !!entry->GetSSL().certificate;
      if (is_same_document) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageSameDocumentIntendedAsNew",
            has_cert);
      } else {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageDifferentDocumentIntendedAsNew",
            has_cert);
      }
    }
  } else if (params.nav_entry_id) {
    entry = GetEntryWithUniqueID(params.nav_entry_id);

    if (is_same_document) {
      NavigationEntryImpl* last_entry = GetLastCommittedEntry();
      if (entry->GetURL().GetOrigin() == last_entry->GetURL().GetOrigin() &&
          last_entry->GetSSL().initialized && !entry->GetSSL().initialized &&
          was_restored) {
        entry->GetSSL() = last_entry->GetSSL();
      }
    } else {
      if (handle->GetSSLInfo().is_valid()) {
        entry->GetSSL() = SSLStatus(handle->GetSSLInfo());
      } else if (entry->GetURL().GetOrigin() != handle->GetURL().GetOrigin()) {
        entry->GetSSL() = SSLStatus();
      }
    }

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      bool has_cert = !!entry->GetSSL().certificate;
      if (is_same_document && was_restored) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageSameDocumentRestoredBrowserInitiated",
            has_cert);
      } else if (is_same_document && !was_restored) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageSameDocumentBrowserInitiated",
            has_cert);
      } else if (!is_same_document && was_restored) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageRestoredBrowserInitiated",
            has_cert);
      } else {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus.ExistingPageBrowserInitiated",
            has_cert);
      }
    }
  } else {
    entry = GetLastCommittedEntry();


    CopyReplacedNavigationEntryDataIfPreviouslyEmpty(*entry, entry);

    if (!is_same_document)
      entry->GetSSL() = SSLStatus(handle->GetSSLInfo());

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      bool has_cert = !!entry->GetSSL().certificate;
      if (is_same_document) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageSameDocumentRendererInitiated",
            has_cert);
      } else {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus."
            "ExistingPageDifferentDocumentRendererInitiated",
            has_cert);
      }
    }
  }
  DCHECK(entry);

  entry->set_page_type(params.url_is_unreachable ? PAGE_TYPE_ERROR
                                                 : PAGE_TYPE_NORMAL);
  entry->SetURL(params.url);
  entry->SetReferrer(params.referrer);
  if (entry->update_virtual_url_with_url())
    UpdateVirtualURLToURL(entry, params.url);

  DCHECK(entry->site_instance() == nullptr ||
         !entry->GetRedirectChain().empty() ||
         entry->site_instance() == rfh->GetSiteInstance());

  entry->AddOrUpdateFrameEntry(
      rfh->frame_tree_node(), params.item_sequence_number,
      params.document_sequence_number, rfh->GetSiteInstance(), nullptr,
      params.url, params.referrer, params.redirects, params.page_state,
      params.method, params.post_id, nullptr  );

  if (ui::PageTransitionIsRedirect(params.transition) && !is_same_document)
    entry->GetFavicon() = FaviconStatus();

  DiscardNonCommittedEntriesInternal();

  last_committed_entry_index_ = GetIndexOfEntry(entry);
}

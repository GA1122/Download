void OfflinePageModelImpl::DeleteExistingPagesWithSameURL(
    const OfflinePageItem& offline_page) {
  size_t pages_allowed =
      policy_controller_->GetPolicy(offline_page.client_id.name_space)
          .pages_allowed_per_url;
  if (pages_allowed == kUnlimitedPages)
    return;
  GetPagesByURL(
      offline_page.url,
      URLSearchMode::SEARCH_BY_FINAL_URL_ONLY,
      base::Bind(&OfflinePageModelImpl::OnPagesFoundWithSameURL,
                 weak_ptr_factory_.GetWeakPtr(), offline_page, pages_allowed));
}

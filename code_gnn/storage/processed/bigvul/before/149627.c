void ResourcePrefetchPredictor::LearnRedirect(const std::string& key,
                                              const GURL& final_redirect,
                                              RedirectDataMap* redirect_data) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (key.length() > ResourcePrefetchPredictorTables::kMaxStringLength)
    return;

  RedirectData data;
  bool exists = redirect_data->TryGetData(key, &data);
  if (!exists) {
    data.set_primary_key(key);
    data.set_last_visit_time(base::Time::Now().ToInternalValue());
    RedirectStat* redirect_to_add = data.add_redirect_endpoints();
    redirect_to_add->set_url(final_redirect.host());
    redirect_to_add->set_number_of_hits(1);
    redirect_to_add->set_url_scheme(final_redirect.scheme());
    redirect_to_add->set_url_port(final_redirect.EffectiveIntPort());
  } else {
    data.set_last_visit_time(base::Time::Now().ToInternalValue());

    bool need_to_add = true;
    for (RedirectStat& redirect : *(data.mutable_redirect_endpoints())) {
      const bool host_mismatch = redirect.url() != final_redirect.host();

      const bool url_scheme_mismatch =
          !redirect.url_scheme().empty() &&
          redirect.url_scheme() != final_redirect.scheme();

      const bool url_port_mismatch =
          redirect.has_url_port() &&
          redirect.url_port() != final_redirect.EffectiveIntPort();

      if (!host_mismatch && !url_scheme_mismatch && !url_port_mismatch) {
        need_to_add = false;
        redirect.set_number_of_hits(redirect.number_of_hits() + 1);
        redirect.set_consecutive_misses(0);

        if (redirect.url_scheme().empty())
          redirect.set_url_scheme(final_redirect.scheme());
        if (!redirect.has_url_port())
          redirect.set_url_port(final_redirect.EffectiveIntPort());
      } else {
        redirect.set_number_of_misses(redirect.number_of_misses() + 1);
        redirect.set_consecutive_misses(redirect.consecutive_misses() + 1);
      }
    }

    if (need_to_add) {
      RedirectStat* redirect_to_add = data.add_redirect_endpoints();
      redirect_to_add->set_url(final_redirect.host());
      redirect_to_add->set_number_of_hits(1);
      redirect_to_add->set_url_scheme(final_redirect.scheme());
      redirect_to_add->set_url_port(final_redirect.EffectiveIntPort());
    }
  }

  ResourcePrefetchPredictorTables::TrimRedirects(
      &data, config_.max_redirect_consecutive_misses);

  if (data.redirect_endpoints_size() == 0)
    redirect_data->DeleteData({key});
  else
    redirect_data->UpdateData(key, data);
}

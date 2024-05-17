bool TemplateURL::MatchesData(const TemplateURL* t_url,
                              const TemplateURLData* data,
                              const SearchTermsData& search_terms_data) {
  if (!t_url || !data)
    return !t_url && !data;

  return (t_url->short_name() == data->short_name()) &&
      t_url->HasSameKeywordAs(*data, search_terms_data) &&
      (t_url->url() == data->url()) &&
      (t_url->suggestions_url() == data->suggestions_url) &&
      (t_url->instant_url() == data->instant_url) &&
      (t_url->image_url() == data->image_url) &&
      (t_url->new_tab_url() == data->new_tab_url) &&
      (t_url->search_url_post_params() == data->search_url_post_params) &&
      (t_url->suggestions_url_post_params() ==
          data->suggestions_url_post_params) &&
      (t_url->instant_url_post_params() == data->instant_url_post_params) &&
      (t_url->image_url_post_params() == data->image_url_post_params) &&
      (t_url->favicon_url() == data->favicon_url) &&
      (t_url->safe_for_autoreplace() == data->safe_for_autoreplace) &&
      (t_url->show_in_default_list() == data->show_in_default_list) &&
      (t_url->input_encodings() == data->input_encodings) &&
      (t_url->alternate_urls() == data->alternate_urls) &&
      (t_url->search_terms_replacement_key() ==
          data->search_terms_replacement_key);
}

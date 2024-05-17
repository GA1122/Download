RedirectStatus GetPredictionRedirectStatus(const GURL& initial_url,
                                           const GURL& main_frame_url,
                                           const std::string& prediction_key,
                                           bool is_redirected,
                                           bool is_host) {
  if (main_frame_url == initial_url) {
    return is_redirected ? RedirectStatus::NO_REDIRECT_BUT_PREDICTED
                         : RedirectStatus::NO_REDIRECT;
  }

  if (!is_redirected)
    return RedirectStatus::REDIRECT_NOT_PREDICTED;

  const std::string& main_frame_key =
      is_host ? main_frame_url.host() : main_frame_url.spec();
  return main_frame_key == prediction_key
             ? RedirectStatus::REDIRECT_CORRECTLY_PREDICTED
             : RedirectStatus::REDIRECT_WRONG_PREDICTED;
}

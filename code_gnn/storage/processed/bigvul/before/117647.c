bool LearnMoreInfoBar::LinkClicked(WindowOpenDisposition disposition) {
  OpenURLParams params(
      learn_more_url_, Referrer(), disposition, content::PAGE_TRANSITION_LINK,
      false);
  owner()->web_contents()->OpenURL(params);
  return false;
}

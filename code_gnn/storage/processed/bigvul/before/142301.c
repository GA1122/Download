bool ChromePasswordManagerClient::IsMainFrameSecure() const {
  return content::IsOriginSecure(web_contents()->GetVisibleURL());
}

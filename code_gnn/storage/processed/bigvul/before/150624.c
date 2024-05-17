void SecureProxyChecker::OnURLLoaderRedirect(
    const net::RedirectInfo& redirect_info,
    const network::ResourceResponseHead& response_head,
    std::vector<std::string>* to_be_removed_headers) {
  OnURLLoadCompleteOrRedirect(std::string(), net::ERR_ABORTED,
                              redirect_info.status_code);
}

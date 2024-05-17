GaiaCookieManagerService::GaiaCookieRequest::GaiaCookieRequest(
    GaiaCookieRequestType request_type,
    const std::string& account_id,
    const std::string& source)
    : request_type_(request_type), account_id_(account_id), source_(source) {}

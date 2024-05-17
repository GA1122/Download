HttpBridge::URLFetchState::URLFetchState() : url_poster(NULL),
                                             aborted(false),
                                             request_completed(false),
                                             request_succeeded(false),
                                             http_response_code(-1),
                                             os_error_code(-1) {}

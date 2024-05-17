std::string FormatUrlErrorMessage(
      const char* format, const GURL& url,
      AppCacheUpdateJob::ResultType error,
      int response_code) {
    int code = response_code;
    if (error != AppCacheUpdateJob::SERVER_ERROR)
      code = static_cast<int>(error);
    return base::StringPrintf(format, code, url.spec().c_str());
}

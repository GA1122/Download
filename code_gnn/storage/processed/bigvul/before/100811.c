bool HttpResponseHeaders::IsRedirectResponseCode(int response_code) {
  return (response_code == 301 ||
          response_code == 302 ||
          response_code == 303 ||
          response_code == 307);
}

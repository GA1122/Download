 bool Job::ResolveDns(const std::string& host,
                      ResolveDnsOperation op,
                      std::string* output,
                     bool* terminate) {
  if (cancelled_.IsSet()) {
    *terminate = true;
    return false;
  }

  if ((op == DNS_RESOLVE || op == DNS_RESOLVE_EX) && host.empty()) {
    return false;
  }

  return blocking_dns_ ?
      ResolveDnsBlocking(host, op, output) :
      ResolveDnsNonBlocking(host, op, output, terminate);
}

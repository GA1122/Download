void Instance::Email(const std::string& to,
                     const std::string& cc,
                     const std::string& bcc,
                     const std::string& subject,
                     const std::string& body) {
  std::string javascript =
      "var href = 'mailto:" + net::EscapeUrlEncodedData(to, false) +
      "?cc=" + net::EscapeUrlEncodedData(cc, false) +
      "&bcc=" + net::EscapeUrlEncodedData(bcc, false) +
      "&subject=" + net::EscapeUrlEncodedData(subject, false) +
      "&body=" + net::EscapeUrlEncodedData(body, false) +
      "';var temp = window.open(href, '_blank', " +
      "'width=1,height=1');if(temp) temp.close();";
  ExecuteScript(javascript);
}

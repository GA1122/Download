void OutOfProcessInstance::Email(const std::string& to,
                                 const std::string& cc,
                                 const std::string& bcc,
                                 const std::string& subject,
                                 const std::string& body) {
  pp::VarDictionary message;
  message.Set(pp::Var(kType), pp::Var(kJSEmailType));
  message.Set(pp::Var(kJSEmailTo),
              pp::Var(net::EscapeUrlEncodedData(to, false)));
  message.Set(pp::Var(kJSEmailCc),
              pp::Var(net::EscapeUrlEncodedData(cc, false)));
  message.Set(pp::Var(kJSEmailBcc),
              pp::Var(net::EscapeUrlEncodedData(bcc, false)));
  message.Set(pp::Var(kJSEmailSubject),
              pp::Var(net::EscapeUrlEncodedData(subject, false)));
  message.Set(pp::Var(kJSEmailBody),
              pp::Var(net::EscapeUrlEncodedData(body, false)));
  PostMessage(message);
}

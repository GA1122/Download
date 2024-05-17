url::Component* GetComponent(url::Parsed* parsed,
                             url::Parsed::ComponentType type) {
  switch (type) {
    case url::Parsed::SCHEME:
      return &parsed->scheme;
    case url::Parsed::USERNAME:
      return &parsed->username;
    case url::Parsed::PASSWORD:
      return &parsed->password;
    case url::Parsed::HOST:
      return &parsed->host;
    case url::Parsed::PORT:
      return &parsed->port;
    case url::Parsed::PATH:
      return &parsed->path;
    case url::Parsed::QUERY:
      return &parsed->query;
    case url::Parsed::REF:
      return &parsed->ref;
    default:
      NOTREACHED();
      return nullptr;
  }
}

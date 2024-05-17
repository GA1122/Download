int Job::ExecuteProxyResolver() {
  int result = ERR_UNEXPECTED;   

  switch (operation_) {
    case CREATE_V8_RESOLVER: {
      scoped_ptr<ProxyResolverV8> resolver;
      result = ProxyResolverV8::Create(script_data_, this, &resolver);
      if (result == OK)
        *resolver_out_ = std::move(resolver);
      break;
    }
    case GET_PROXY_FOR_URL: {
      result = v8_resolver()->GetProxyForURL(
          url_,
          &results_, this);
      break;
    }
  }

   return result;
 }

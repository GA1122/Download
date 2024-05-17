void PrepareHttpResponse(const Response& command_response,
                         HttpResponse* const http_response) {
  ErrorCode status = command_response.GetStatus();
  switch (status) {
    case kSuccess:
      http_response->set_status(HttpResponse::kOk);
      break;

    case kSeeOther: {
      const Value* const value = command_response.GetValue();
      std::string location;
      if (!value->GetAsString(&location)) {
        http_response->set_status(HttpResponse::kInternalServerError);
        http_response->SetBody("Unable to set 'Location' header: response "
                               "value is not a string: " +
                               command_response.ToJSON());
        return;
      }
      http_response->AddHeader("Location", location);
      http_response->set_status(HttpResponse::kSeeOther);
      break;
    }

    case kBadRequest:
    case kSessionNotFound:
      http_response->set_status(status);
      break;

    case kMethodNotAllowed: {
      const Value* const value = command_response.GetValue();
      if (!value->IsType(Value::TYPE_LIST)) {
        http_response->set_status(HttpResponse::kInternalServerError);
        http_response->SetBody(
            "Unable to set 'Allow' header: response value was "
            "not a list of strings: " + command_response.ToJSON());
        return;
      }

      const ListValue* const list_value =
          static_cast<const ListValue* const>(value);
      std::vector<std::string> allowed_methods;
      for (size_t i = 0; i < list_value->GetSize(); ++i) {
        std::string method;
        if (list_value->GetString(i, &method)) {
          allowed_methods.push_back(method);
        } else {
          http_response->set_status(HttpResponse::kInternalServerError);
          http_response->SetBody(
              "Unable to set 'Allow' header: response value was "
              "not a list of strings: " + command_response.ToJSON());
          return;
        }
      }
      http_response->AddHeader("Allow", JoinString(allowed_methods, ','));
      http_response->set_status(HttpResponse::kMethodNotAllowed);
      break;
    }

    case kInternalServerError:
    default:
      http_response->set_status(HttpResponse::kInternalServerError);
      break;
  }

  http_response->SetMimeType("application/json; charset=utf-8");
  http_response->SetBody(command_response.ToJSON());
}

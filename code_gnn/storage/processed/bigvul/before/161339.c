String NetErrorToString(int net_error) {
  if (net_error == net::ERR_ABORTED)
    return Network::ErrorReasonEnum::Aborted;
  if (net_error == net::ERR_TIMED_OUT)
    return Network::ErrorReasonEnum::TimedOut;
  if (net_error == net::ERR_ACCESS_DENIED)
    return Network::ErrorReasonEnum::AccessDenied;
  if (net_error == net::ERR_CONNECTION_CLOSED)
    return Network::ErrorReasonEnum::ConnectionClosed;
  if (net_error == net::ERR_CONNECTION_RESET)
    return Network::ErrorReasonEnum::ConnectionReset;
  if (net_error == net::ERR_CONNECTION_REFUSED)
    return Network::ErrorReasonEnum::ConnectionRefused;
  if (net_error == net::ERR_CONNECTION_ABORTED)
    return Network::ErrorReasonEnum::ConnectionAborted;
  if (net_error == net::ERR_CONNECTION_FAILED)
    return Network::ErrorReasonEnum::ConnectionFailed;
  if (net_error == net::ERR_NAME_NOT_RESOLVED)
    return Network::ErrorReasonEnum::NameNotResolved;
  if (net_error == net::ERR_INTERNET_DISCONNECTED)
    return Network::ErrorReasonEnum::InternetDisconnected;
  if (net_error == net::ERR_ADDRESS_UNREACHABLE)
    return Network::ErrorReasonEnum::AddressUnreachable;
  return Network::ErrorReasonEnum::Failed;
}

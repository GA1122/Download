net::Error NetErrorFromString(const std::string& error, bool* ok) {
  *ok = true;
  if (error == Network::ErrorReasonEnum::Failed)
    return net::ERR_FAILED;
  if (error == Network::ErrorReasonEnum::Aborted)
    return net::ERR_ABORTED;
  if (error == Network::ErrorReasonEnum::TimedOut)
    return net::ERR_TIMED_OUT;
  if (error == Network::ErrorReasonEnum::AccessDenied)
    return net::ERR_ACCESS_DENIED;
  if (error == Network::ErrorReasonEnum::ConnectionClosed)
    return net::ERR_CONNECTION_CLOSED;
  if (error == Network::ErrorReasonEnum::ConnectionReset)
    return net::ERR_CONNECTION_RESET;
  if (error == Network::ErrorReasonEnum::ConnectionRefused)
    return net::ERR_CONNECTION_REFUSED;
  if (error == Network::ErrorReasonEnum::ConnectionAborted)
    return net::ERR_CONNECTION_ABORTED;
  if (error == Network::ErrorReasonEnum::ConnectionFailed)
    return net::ERR_CONNECTION_FAILED;
  if (error == Network::ErrorReasonEnum::NameNotResolved)
    return net::ERR_NAME_NOT_RESOLVED;
  if (error == Network::ErrorReasonEnum::InternetDisconnected)
    return net::ERR_INTERNET_DISCONNECTED;
  if (error == Network::ErrorReasonEnum::AddressUnreachable)
    return net::ERR_ADDRESS_UNREACHABLE;
  *ok = false;
  return net::ERR_FAILED;
}

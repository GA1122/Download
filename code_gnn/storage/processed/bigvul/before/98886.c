bool WebSocketHandshake::CheckResponseHeaders() const {
  DCHECK(mode_ == MODE_NORMAL);
  if (!LowerCaseEqualsASCII(origin_, ws_origin_.c_str()))
    return false;
  if (location_ != ws_location_)
    return false;
  if (!protocol_.empty() && protocol_ != ws_protocol_)
    return false;
   return true;
 }

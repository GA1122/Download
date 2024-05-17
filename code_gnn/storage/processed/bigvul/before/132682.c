void BlinkTestRunner::SetPermission(const std::string& name,
                                    const std::string& value,
                                    const GURL& origin,
                                    const GURL& embedding_origin) {
  content::PermissionStatus status;
  if (value == "granted")
    status = PERMISSION_STATUS_GRANTED;
  else if (value == "prompt")
    status = PERMISSION_STATUS_ASK;
  else if (value == "denied")
    status = PERMISSION_STATUS_DENIED;
  else {
    NOTREACHED();
    status = PERMISSION_STATUS_DENIED;
  }

  Send(new LayoutTestHostMsg_SetPermission(
      routing_id(), name, status, origin, embedding_origin));
}

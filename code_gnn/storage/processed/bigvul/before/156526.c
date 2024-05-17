bool ChildProcessSecurityPolicyImpl::CanReadRequestBody(
    int child_id,
    const storage::FileSystemContext* file_system_context,
    const scoped_refptr<network::ResourceRequestBody>& body) {
  if (!body)
    return true;

  for (const network::DataElement& element : *body->elements()) {
    switch (element.type()) {
      case network::DataElement::TYPE_FILE:
        if (!CanReadFile(child_id, element.path()))
          return false;
        break;

      case network::DataElement::TYPE_BYTES:
        break;

      case network::DataElement::TYPE_BLOB:
        break;

      case network::DataElement::TYPE_DATA_PIPE:
        break;

      case network::DataElement::TYPE_UNKNOWN:
      default:
        NOTREACHED();
        return false;
    }
  }
  return true;
}

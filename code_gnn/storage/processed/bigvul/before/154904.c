bool ModifyAttachmentsForEmulatedFramebuffer(std::vector<GLenum>* attachments) {
  for (GLenum& attachment : *attachments) {
    if (!ModifyAttachmentForEmulatedFramebuffer(&attachment)) {
      return false;
    }
  }

  return true;
}

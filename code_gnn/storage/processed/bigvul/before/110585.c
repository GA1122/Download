error::Error GLES2DecoderImpl::HandleGenMailboxCHROMIUM(
    uint32 immediate_data_size, const gles2::GenMailboxCHROMIUM& c) {
  MailboxName name;
  mailbox_manager()->GenerateMailboxName(&name);
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);

  bucket->SetSize(GL_MAILBOX_SIZE_CHROMIUM);
  bucket->SetData(&name, 0, GL_MAILBOX_SIZE_CHROMIUM);

  return error::kNoError;
}

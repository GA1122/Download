void Resource::CheckResourceIntegrity() {
  if (IntegrityDisposition() != ResourceIntegrityDisposition::kNotChecked)
    return;

  integrity_report_info_.Clear();
  if (ErrorOccurred()) {
    CHECK(!Data());
    integrity_disposition_ = ResourceIntegrityDisposition::kFailed;
    return;
  }

  if (IntegrityMetadata().IsEmpty()) {
    integrity_disposition_ = ResourceIntegrityDisposition::kPassed;
    return;
  }

  const char* data = nullptr;
  size_t data_length = 0;

  CHECK(DecodedSize() == 0 || Data());
  if (Data()) {
    data = Data()->Data();
    data_length = Data()->size();
  }

  if (SubresourceIntegrity::CheckSubresourceIntegrity(IntegrityMetadata(), data,
                                                      data_length, Url(), *this,
                                                      integrity_report_info_))
    integrity_disposition_ = ResourceIntegrityDisposition::kPassed;
  else
    integrity_disposition_ = ResourceIntegrityDisposition::kFailed;
  DCHECK_NE(IntegrityDisposition(), ResourceIntegrityDisposition::kNotChecked);
}

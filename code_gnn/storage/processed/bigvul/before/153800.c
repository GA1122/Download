void GLES2Implementation::RequestExtensionCHROMIUM(const char* extension) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glRequestExtensionCHROMIUM("
                     << extension << ")");
  InvalidateCachedExtensions();
  SetBucketAsCString(kResultBucketId, extension);
  helper_->RequestExtensionCHROMIUM(kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);

  struct ExtensionCheck {
    const char* extension;
    ExtensionStatus* status;
  };
  const ExtensionCheck checks[] = {
      {
          "GL_CHROMIUM_framebuffer_multisample",
          &chromium_framebuffer_multisample_,
      },
  };
  const size_t kNumChecks = sizeof(checks) / sizeof(checks[0]);
  for (size_t ii = 0; ii < kNumChecks; ++ii) {
    const ExtensionCheck& check = checks[ii];
    if (*check.status == kUnavailableExtensionStatus &&
        !strcmp(extension, check.extension)) {
      *check.status = kUnknownExtensionStatus;
    }
  }
}

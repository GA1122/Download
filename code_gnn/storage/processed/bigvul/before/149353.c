void BinaryUploadService::OnGetRequestData(Request* request,
                                           Result result,
                                           const Request::Data& data) {
  if (!IsActive(request))
    return;

  if (result != Result::SUCCESS) {
    FinishRequest(request, result, DeepScanningClientResponse());
    return;
  }

  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("safe_browsing_binary_upload", R"(
        semantics {
          sender: "Safe Browsing Download Protection"
          description:
            "For users with the enterprise policy "
            "SendFilesForMalwareCheck set, when a file is "
            "downloaded, Chrome will upload that file to Safe Browsing for "
            "detailed scanning."
          trigger:
            "The browser will upload the file to Google when "
            "the user downloads a file, and the enterprise policy "
            "SendFilesForMalwareCheck is set."
          data:
            "The downloaded file."
          destination: GOOGLE_OWNED_SERVICE
        }
        policy {
          cookies_allowed: YES
          cookies_store: "Safe Browsing Cookie Store"
          setting: "This is disabled by default an can only be enabled by "
            "policy."
          chrome_policy {
            SendFilesForMalwareCheck {
              SendFilesForMalwareCheck: 0
            }
          }
          chrome_policy {
            SendFilesForMalwareCheck {
              SendFilesForMalwareCheck: 1
            }
          }
        }
        comments: "Setting SendFilesForMalwareCheck to 0 (Do not scan "
          "downloads) or 1 (Forbid the scanning of downloads) will disable "
          "this feature"
        )");

  std::string metadata;
  request->deep_scanning_request().SerializeToString(&metadata);
  base::Base64Encode(metadata, &metadata);

  auto upload_request = MultipartUploadRequest::Create(
      url_loader_factory_, GURL(kSbBinaryUploadUrl), metadata, data.contents,
      traffic_annotation,
      base::BindOnce(&BinaryUploadService::OnUploadComplete,
                     weakptr_factory_.GetWeakPtr(), request));
  upload_request->Start();
  active_uploads_[request] = std::move(upload_request);
}

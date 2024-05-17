void ReportUsageAndQuotaDataOnUIThread(
    std::unique_ptr<StorageHandler::GetUsageAndQuotaCallback> callback,
    blink::mojom::QuotaStatusCode code,
    int64_t usage,
    int64_t quota,
    base::flat_map<storage::QuotaClient::ID, int64_t> usage_breakdown) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (code != blink::mojom::QuotaStatusCode::kOk) {
    return callback->sendFailure(
        Response::Error("Quota information is not available"));
  }

  std::unique_ptr<Array<Storage::UsageForType>> usageList =
      Array<Storage::UsageForType>::create();
  for (const auto& specific_usage : usage_breakdown) {
    std::unique_ptr<Storage::UsageForType> entry =
        Storage::UsageForType::Create()
            .SetStorageType(GetTypeName(specific_usage.first))
            .SetUsage(specific_usage.second)
            .Build();
    usageList->addItem(std::move(entry));
  }
  callback->sendSuccess(usage, quota, std::move(usageList));
}

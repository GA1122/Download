void StoragePartitionImpl::OnCanSendDomainReliabilityUpload(
    const GURL& origin,
    OnCanSendDomainReliabilityUploadCallback callback) {
  PermissionController* permission_controller =
      BrowserContext::GetPermissionController(browser_context_);
  std::move(callback).Run(
      permission_controller->GetPermissionStatus(
          content::PermissionType::BACKGROUND_SYNC, origin, origin) ==
      blink::mojom::PermissionStatus::GRANTED);
}

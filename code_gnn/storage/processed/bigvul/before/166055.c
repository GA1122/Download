void FileSystemManagerImpl::ChooseEntry(
    blink::mojom::ChooseFileSystemEntryType type,
    std::vector<blink::mojom::ChooseFileSystemEntryAcceptsOptionPtr> accepts,
    bool include_accepts_all,
    ChooseEntryCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!base::FeatureList::IsEnabled(blink::features::kWritableFilesAPI)) {
    bindings_.ReportBadMessage("FSMI_WRITABLE_FILES_DISABLED");
    return;
  }

  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(
          &FileSystemChooser::CreateAndShow, process_id_, frame_id_, type,
          std::move(accepts), include_accepts_all, std::move(callback),
          base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::IO})));
}

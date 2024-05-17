void PrintViewManagerBase::OnShowInvalidPrinterSettingsError() {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&ShowWarningMessageBox,
                                l10n_util::GetStringUTF16(
                                    IDS_PRINT_INVALID_PRINTER_SETTINGS)));
}

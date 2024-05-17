PageSnapshotTaker::PageSnapshotTaker(AutomationProvider* automation,
                                     IPC::Message* reply_message,
                                     TabContents* tab_contents,
                                     const FilePath& path)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      tab_contents_(tab_contents),
      image_path_(path) {
  registrar_.Add(this, chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
                 content::NotificationService::AllSources());
}

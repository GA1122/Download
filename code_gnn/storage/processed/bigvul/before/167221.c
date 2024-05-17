base::string16 ExtensionDevToolsInfoBarDelegate::GetMessageText() const {
   return l10n_util::GetStringFUTF16(IDS_DEV_TOOLS_INFOBAR_LABEL, client_name_);
 }

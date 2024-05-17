BOOL Notification(int type, const notification_info* more_info, char* title, char* format, ...)
{
	BOOL ret;
	va_list args;

	dialog_showing++;
	szMessageText = (char*)malloc(MAX_PATH);
	if (szMessageText == NULL) return FALSE;
	szMessageTitle = safe_strdup(title);
	if (szMessageTitle == NULL) return FALSE;
	va_start(args, format);
	safe_vsnprintf(szMessageText, MAX_PATH-1, format, args);
	va_end(args);
	szMessageText[MAX_PATH-1] = 0;
	notification_more_info = more_info;
	notification_is_question = FALSE;

	switch(type) {
	case MSG_WARNING_QUESTION:
		notification_is_question = TRUE;
	case MSG_WARNING:
		hMessageIcon = LoadIcon(NULL, IDI_WARNING);
		break;
	case MSG_ERROR:
		hMessageIcon = LoadIcon(NULL, IDI_ERROR);
		break;
	case MSG_QUESTION:
		hMessageIcon = LoadIcon(NULL, IDI_QUESTION);
		notification_is_question = TRUE;
		break;
	case MSG_INFO:
	default:
		hMessageIcon = LoadIcon(NULL, IDI_INFORMATION);
		break;
	}
	ret = (MyDialogBox(hMainInstance, IDD_NOTIFICATION, hMainDialog, NotificationCallback) == IDYES);
	safe_free(szMessageText);
	safe_free(szMessageTitle);
	dialog_showing--;
	return ret;
}
body_cmd(thread_t * thread)
{
	smtp_t *smtp = THREAD_ARG(thread);
	char *buffer;
	char rfc822[80];
	time_t now;
	struct tm *t;

	buffer = (char *) MALLOC(SMTP_BUFFER_MAX);

	time(&now);
	t = localtime(&now);
	strftime(rfc822, sizeof(rfc822), "%a, %d %b %Y %H:%M:%S %z", t);

	snprintf(buffer, SMTP_BUFFER_MAX, SMTP_HEADERS_CMD,
		 rfc822, global_data->email_from, smtp->subject, smtp->email_to);

	 
	if (send(thread->u.fd, buffer, strlen(buffer), 0) == -1)
		smtp->stage = ERROR;

	memset(buffer, 0, SMTP_BUFFER_MAX);
	snprintf(buffer, SMTP_BUFFER_MAX, SMTP_BODY_CMD, smtp->body);

	 
	if (send(thread->u.fd, buffer, strlen(buffer), 0) == -1)
		smtp->stage = ERROR;

	 
	if (send(thread->u.fd, SMTP_SEND_CMD, strlen(SMTP_SEND_CMD), 0) == -1)
		smtp->stage = ERROR;

	FREE(buffer);
	return 0;
}
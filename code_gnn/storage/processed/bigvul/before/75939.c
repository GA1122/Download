rcpt_cmd(thread_t * thread)
{
	smtp_t *smtp = THREAD_ARG(thread);
	char *buffer;
	char *fetched_email;

	buffer = (char *) MALLOC(SMTP_BUFFER_MAX);
	 
	fetched_email = fetch_next_email(smtp);

	snprintf(buffer, SMTP_BUFFER_MAX, SMTP_RCPT_CMD, fetched_email);
	if (send(thread->u.fd, buffer, strlen(buffer), 0) == -1)
		smtp->stage = ERROR;
	FREE(buffer);

	return 0;
}

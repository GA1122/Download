build_to_header_rcpt_addrs(smtp_t *smtp)
{
	char *fetched_email;
	char *email_to_addrs;
	size_t bytes_available = SMTP_BUFFER_MAX - 1;
	size_t bytes_to_write;

	if (smtp == NULL)
		return;

	email_to_addrs = smtp->email_to;
	smtp->email_it = 0;

	while (1) {
		fetched_email = fetch_next_email(smtp);
		if (fetched_email == NULL)
			break;

		bytes_to_write = strlen(fetched_email);
		if (!smtp->email_it) {
			if (bytes_available < bytes_to_write)
				break;
		} else {
			if (bytes_available < 2 + bytes_to_write)
				break;

			 
			*email_to_addrs++ = ',';
			*email_to_addrs++ = ' ';
			bytes_available -= 2;
		}

		if (snprintf(email_to_addrs, bytes_to_write + 1, "%s", fetched_email) != (int)bytes_to_write) {
			 
			break;
		}

		email_to_addrs += bytes_to_write;
		bytes_available -= bytes_to_write;
		smtp->email_it++;
	}

	smtp->email_it = 0;
}
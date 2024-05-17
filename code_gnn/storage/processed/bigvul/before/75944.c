smtp_send_thread(thread_t * thread)
{
	smtp_t *smtp = THREAD_ARG(thread);

	if (thread->type == THREAD_WRITE_TIMEOUT) {
		log_message(LOG_INFO, "Timeout sending data to remote SMTP server %s."
				    , FMT_SMTP_HOST());
		SMTP_FSM_READ(QUIT, thread, 0);
		return 0;
	}

	SMTP_FSM_SEND(smtp->stage, thread);

	 
	if (smtp->stage == END) {
		SMTP_FSM_READ(QUIT, thread, 0);
		return 0;
	}

	 
	if (smtp->stage != ERROR) {
		thread_add_read(thread->master, smtp_read_thread, smtp,
				thread->u.fd, global_data->smtp_connection_to);
		thread_del_write(thread);
	} else {
		log_message(LOG_INFO, "Can not send data to remote SMTP server %s."
				    , FMT_SMTP_HOST());
		SMTP_FSM_READ(QUIT, thread, 0);
	}

	return 0;
}
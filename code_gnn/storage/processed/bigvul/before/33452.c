void kdb_syslog_data(char *syslog_data[4])
{
	syslog_data[0] = log_buf;
	syslog_data[1] = log_buf + log_buf_len;
	syslog_data[2] = log_buf + log_end -
		(logged_chars < log_buf_len ? logged_chars : log_buf_len);
	syslog_data[3] = log_buf + log_end;
}
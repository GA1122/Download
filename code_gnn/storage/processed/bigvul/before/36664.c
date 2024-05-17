json_t *json_web_config(const char *url)
{
	struct data_buffer all_data = {NULL, 0};
	char curl_err_str[CURL_ERROR_SIZE];
	long timeout = 60;
	json_error_t err;
	json_t *val;
	CURL *curl;
	int rc;

	memset(&err, 0, sizeof(err));

	curl = curl_easy_init();
	if (unlikely(!curl))
		quithere(1, "CURL initialisation failed");

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_ENCODING, "");
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, all_data_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &all_data);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_err_str);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);

	val = NULL;
	rc = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if (rc) {
		applog(LOG_ERR, "HTTP config request of '%s' failed: %s", url, curl_err_str);
		goto c_out;
	}

	if (!all_data.buf) {
		applog(LOG_ERR, "Empty config data received from '%s'", url);
		goto c_out;
	}

	val = JSON_LOADS(all_data.buf, &err);
	if (!val) {
		applog(LOG_ERR, "JSON config decode of '%s' failed(%d): %s", url,
		       err.line, err.text);
	}
	databuf_free(&all_data);

c_out:
	return val;
}

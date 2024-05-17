static void keep_curlalive(CURL *curl)
{
	const int tcp_keepidle = 45;
	const int tcp_keepintvl = 30;
	const long int keepalive = 1;

	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, keepalive);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, tcp_keepidle);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, tcp_keepintvl);
}

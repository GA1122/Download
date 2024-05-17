server_abort_http(struct client *clt, unsigned int code, const char *msg)
{
	struct server_config	*srv_conf = clt->clt_srv_conf;
	struct bufferevent	*bev = clt->clt_bev;
	struct http_descriptor	*desc = clt->clt_descreq;
	const char		*httperr = NULL, *style;
	char			*httpmsg, *body = NULL, *extraheader = NULL;
	char			 tmbuf[32], hbuf[128], *hstsheader = NULL;
	char			 buf[IBUF_READ_SIZE];
	char			*escapedmsg = NULL;
	int			 bodylen;

	if (code == 0) {
		server_close(clt, "dropped");
		return;
	}

	if ((httperr = server_httperror_byid(code)) == NULL)
		httperr = "Unknown Error";

	if (bev == NULL)
		goto done;

	if (server_log_http(clt, code, 0) == -1)
		goto done;

	 
	if (print_host(&srv_conf->ss, hbuf, sizeof(hbuf)) == NULL)
		goto done;

	if (server_http_time(time(NULL), tmbuf, sizeof(tmbuf)) <= 0)
		goto done;

	 
	switch (code) {
	case 301:
	case 302:
	case 303:
		if (msg == NULL)
			break;
		memset(buf, 0, sizeof(buf));
		if (server_expand_http(clt, msg, buf, sizeof(buf)) == NULL)
			goto done;
		if (asprintf(&extraheader, "Location: %s\r\n", buf) == -1) {
			code = 500;
			extraheader = NULL;
		}
		msg = buf;
		break;
	case 401:
		if (stravis(&escapedmsg, msg, VIS_DQ) == -1) {
			code = 500;
			extraheader = NULL;
		} else if (asprintf(&extraheader,
		    "WWW-Authenticate: Basic realm=\"%s\"\r\n", escapedmsg)
		    == -1) {
			code = 500;
			extraheader = NULL;
		}
		break;
	case 416:
		if (asprintf(&extraheader,
		    "Content-Range: %s\r\n", msg) == -1) {
			code = 500;
			extraheader = NULL;
		}
		break;
	default:
		 
		break;
	}

	free(escapedmsg);

	 
	style = "body { background-color: white; color: black; font-family: "
	    "'Comic Sans MS', 'Chalkboard SE', 'Comic Neue', sans-serif; }\n"
	    "hr { border: 0; border-bottom: 1px dashed; }\n";

	 
	if ((bodylen = asprintf(&body,
	    "<!DOCTYPE html>\n"
	    "<html>\n"
	    "<head>\n"
	    "<meta http-equiv=\"Content-Type\" content=\"text/html; "
	    "charset=utf-8\"/>\n"
	    "<title>%03d %s</title>\n"
	    "<style type=\"text/css\"><!--\n%s\n--></style>\n"
	    "</head>\n"
	    "<body>\n"
	    "<h1>%03d %s</h1>\n"
	    "<hr>\n<address>%s</address>\n"
	    "</body>\n"
	    "</html>\n",
	    code, httperr, style, code, httperr, HTTPD_SERVERNAME)) == -1) {
		body = NULL;
		goto done;
	}

	if (srv_conf->flags & SRVFLAG_SERVER_HSTS) {
		if (asprintf(&hstsheader, "Strict-Transport-Security: "
		    "max-age=%d%s%s\r\n", srv_conf->hsts_max_age,
		    srv_conf->hsts_flags & HSTSFLAG_SUBDOMAINS ?
		    "; includeSubDomains" : "",
		    srv_conf->hsts_flags & HSTSFLAG_PRELOAD ?
		    "; preload" : "") == -1) {
			hstsheader = NULL;
			goto done;
		}
	}

	 
	if (asprintf(&httpmsg,
	    "HTTP/1.0 %03d %s\r\n"
	    "Date: %s\r\n"
	    "Server: %s\r\n"
	    "Connection: close\r\n"
	    "Content-Type: text/html\r\n"
	    "Content-Length: %d\r\n"
	    "%s"
	    "%s"
	    "\r\n"
	    "%s",
	    code, httperr, tmbuf, HTTPD_SERVERNAME, bodylen,
	    extraheader == NULL ? "" : extraheader,
	    hstsheader == NULL ? "" : hstsheader,
	    desc->http_method == HTTP_METHOD_HEAD ? "" : body) == -1)
		goto done;

	 
	server_dump(clt, httpmsg, strlen(httpmsg));
	free(httpmsg);

 done:
	free(body);
	free(extraheader);
	free(hstsheader);
	if (msg == NULL)
		msg = "\"\"";
	if (asprintf(&httpmsg, "%s (%03d %s)", msg, code, httperr) == -1) {
		server_close(clt, msg);
	} else {
		server_close(clt, httpmsg);
		free(httpmsg);
	}
}
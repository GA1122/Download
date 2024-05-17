 static int process_line(URLContext *h, char *line, int line_count,
                         int *new_location)
 {
     HTTPContext *s = h->priv_data;
     const char *auto_method =  h->flags & AVIO_FLAG_READ ? "POST" : "GET";
     char *tag, *p, *end, *method, *resource, *version;
     int ret;
 
      
     if (line[0] == '\0') {
         s->end_header = 1;
         return 0;
     }
 
     p = line;
     if (line_count == 0) {
         if (s->is_connected_server) {
             method = p;
             while (*p && !av_isspace(*p))
                 p++;
             *(p++) = '\0';
             av_log(h, AV_LOG_TRACE, "Received method: %s\n", method);
             if (s->method) {
                 if (av_strcasecmp(s->method, method)) {
                     av_log(h, AV_LOG_ERROR, "Received and expected HTTP method do not match. (%s expected, %s received)\n",
                            s->method, method);
                     return ff_http_averror(400, AVERROR(EIO));
                 }
             } else {
                 av_log(h, AV_LOG_TRACE, "Autodetected %s HTTP method\n", auto_method);
                 if (av_strcasecmp(auto_method, method)) {
                     av_log(h, AV_LOG_ERROR, "Received and autodetected HTTP method did not match "
                            "(%s autodetected %s received)\n", auto_method, method);
                     return ff_http_averror(400, AVERROR(EIO));
                 }
                 if (!(s->method = av_strdup(method)))
                     return AVERROR(ENOMEM);
             }
 
             while (av_isspace(*p))
                 p++;
             resource = p;
             while (!av_isspace(*p))
                 p++;
             *(p++) = '\0';
             av_log(h, AV_LOG_TRACE, "Requested resource: %s\n", resource);
             if (!(s->resource = av_strdup(resource)))
                 return AVERROR(ENOMEM);
 
             while (av_isspace(*p))
                 p++;
             version = p;
             while (*p && !av_isspace(*p))
                 p++;
             *p = '\0';
             if (av_strncasecmp(version, "HTTP/", 5)) {
                 av_log(h, AV_LOG_ERROR, "Malformed HTTP version string.\n");
                 return ff_http_averror(400, AVERROR(EIO));
             }
             av_log(h, AV_LOG_TRACE, "HTTP version string: %s\n", version);
         } else {
             while (!av_isspace(*p) && *p != '\0')
                 p++;
             while (av_isspace(*p))
                 p++;
             s->http_code = strtol(p, &end, 10);
 
             av_log(h, AV_LOG_TRACE, "http_code=%d\n", s->http_code);
 
             if ((ret = check_http_code(h, s->http_code, end)) < 0)
                 return ret;
         }
     } else {
         while (*p != '\0' && *p != ':')
             p++;
         if (*p != ':')
             return 1;
 
         *p  = '\0';
         tag = line;
         p++;
         while (av_isspace(*p))
             p++;
         if (!av_strcasecmp(tag, "Location")) {
              if ((ret = parse_location(s, p)) < 0)
                  return ret;
              *new_location = 1;
        } else if (!av_strcasecmp(tag, "Content-Length") && s->filesize == -1) {
            s->filesize = strtoll(p, NULL, 10);
//         } else if (!av_strcasecmp(tag, "Content-Length") &&
//                    s->filesize == UINT64_MAX) {
//             s->filesize = strtoull(p, NULL, 10);
          } else if (!av_strcasecmp(tag, "Content-Range")) {
              parse_content_range(h, p);
          } else if (!av_strcasecmp(tag, "Accept-Ranges") &&
                    !strncmp(p, "bytes", 5) &&
                    s->seekable == -1) {
              h->is_streamed = 0;
          } else if (!av_strcasecmp(tag, "Transfer-Encoding") &&
                     !av_strncasecmp(p, "chunked", 7)) {
            s->filesize  = -1;
//             s->filesize  = UINT64_MAX;
              s->chunksize = 0;
          } else if (!av_strcasecmp(tag, "WWW-Authenticate")) {
              ff_http_auth_handle_header(&s->auth_state, tag, p);
         } else if (!av_strcasecmp(tag, "Authentication-Info")) {
             ff_http_auth_handle_header(&s->auth_state, tag, p);
         } else if (!av_strcasecmp(tag, "Proxy-Authenticate")) {
             ff_http_auth_handle_header(&s->proxy_auth_state, tag, p);
         } else if (!av_strcasecmp(tag, "Connection")) {
             if (!strcmp(p, "close"))
                 s->willclose = 1;
         } else if (!av_strcasecmp(tag, "Server")) {
             if (!av_strcasecmp(p, "AkamaiGHost")) {
                 s->is_akamai = 1;
             } else if (!av_strncasecmp(p, "MediaGateway", 12)) {
                 s->is_mediagateway = 1;
             }
         } else if (!av_strcasecmp(tag, "Content-Type")) {
             av_free(s->mime_type);
             s->mime_type = av_strdup(p);
         } else if (!av_strcasecmp(tag, "Set-Cookie")) {
              if (parse_cookie(s, p, &s->cookie_dict))
                  av_log(h, AV_LOG_WARNING, "Unable to parse '%s'\n", p);
          } else if (!av_strcasecmp(tag, "Icy-MetaInt")) {
            s->icy_metaint = strtoll(p, NULL, 10);
//             s->icy_metaint = strtoull(p, NULL, 10);
          } else if (!av_strncasecmp(tag, "Icy-", 4)) {
              if ((ret = parse_icy(s, tag, p)) < 0)
                  return ret;
         } else if (!av_strcasecmp(tag, "Content-Encoding")) {
             if ((ret = parse_content_encoding(h, p)) < 0)
                 return ret;
         }
     }
     return 1;
 }
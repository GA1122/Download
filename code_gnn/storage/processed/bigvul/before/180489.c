 send_results(struct iperf_test *test)
 {
     int r = 0;
     cJSON *j;
     cJSON *j_streams;
     struct iperf_stream *sp;
     cJSON *j_stream;
     int sender_has_retransmits;
     iperf_size_t bytes_transferred;
     int retransmits;
 
     j = cJSON_CreateObject();
     if (j == NULL) {
  	i_errno = IEPACKAGERESULTS;
  	r = -1;
      } else {
	cJSON_AddFloatToObject(j, "cpu_util_total", test->cpu_util[0]);
	cJSON_AddFloatToObject(j, "cpu_util_user", test->cpu_util[1]);
	cJSON_AddFloatToObject(j, "cpu_util_system", test->cpu_util[2]);
// 	cJSON_AddNumberToObject(j, "cpu_util_total", test->cpu_util[0]);
// 	cJSON_AddNumberToObject(j, "cpu_util_user", test->cpu_util[1]);
// 	cJSON_AddNumberToObject(j, "cpu_util_system", test->cpu_util[2]);
  	if ( ! test->sender )
  	    sender_has_retransmits = -1;
  	else
  	    sender_has_retransmits = test->sender_has_retransmits;
	cJSON_AddIntToObject(j, "sender_has_retransmits", sender_has_retransmits);
// 	cJSON_AddNumberToObject(j, "sender_has_retransmits", sender_has_retransmits);
  
  	 
  	if (test->role == 's' && test->get_server_output) {
 	    if (test->json_output) {
 		 
 		cJSON_AddItemReferenceToObject(j, "server_output_json", test->json_top);
 	    }
 	    else {
 		 
 		size_t buflen = 0;
 
 		 
 		struct iperf_textline *t;
 		TAILQ_FOREACH(t, &(test->server_output_list), textlineentries) {
 		    buflen += strlen(t->line);
 		}
 
 		 
 		char *output = calloc(buflen + 1, 1);
 		TAILQ_FOREACH(t, &(test->server_output_list), textlineentries) {
 		    strncat(output, t->line, buflen);
 		    buflen -= strlen(t->line);
 		}
 
 		cJSON_AddStringToObject(j, "server_output_text", output);
 	    }
 	}
 
 	j_streams = cJSON_CreateArray();
 	if (j_streams == NULL) {
 	    i_errno = IEPACKAGERESULTS;
 	    r = -1;
 	} else {
 	    cJSON_AddItemToObject(j, "streams", j_streams);
 	    SLIST_FOREACH(sp, &test->streams, streams) {
 		j_stream = cJSON_CreateObject();
 		if (j_stream == NULL) {
 		    i_errno = IEPACKAGERESULTS;
 		    r = -1;
 		} else {
  		    cJSON_AddItemToArray(j_streams, j_stream);
  		    bytes_transferred = test->sender ? sp->result->bytes_sent : sp->result->bytes_received;
  		    retransmits = (test->sender && test->sender_has_retransmits) ? sp->result->stream_retrans : -1;
		    cJSON_AddIntToObject(j_stream, "id", sp->id);
		    cJSON_AddIntToObject(j_stream, "bytes", bytes_transferred);
		    cJSON_AddIntToObject(j_stream, "retransmits", retransmits);
		    cJSON_AddFloatToObject(j_stream, "jitter", sp->jitter);
		    cJSON_AddIntToObject(j_stream, "errors", sp->cnt_error);
		    cJSON_AddIntToObject(j_stream, "packets", sp->packet_count);
// 		    cJSON_AddNumberToObject(j_stream, "id", sp->id);
// 		    cJSON_AddNumberToObject(j_stream, "bytes", bytes_transferred);
// 		    cJSON_AddNumberToObject(j_stream, "retransmits", retransmits);
// 		    cJSON_AddNumberToObject(j_stream, "jitter", sp->jitter);
// 		    cJSON_AddNumberToObject(j_stream, "errors", sp->cnt_error);
// 		    cJSON_AddNumberToObject(j_stream, "packets", sp->packet_count);
  		}
  	    }
  	    if (r == 0 && test->debug) {
 		printf("send_results\n%s\n", cJSON_Print(j));
 	    }
 	    if (r == 0 && JSON_write(test->ctrl_sck, j) < 0) {
 		i_errno = IESENDRESULTS;
 		r = -1;
 	    }
 	}
 	cJSON_Delete(j);
     }
     return r;
 }
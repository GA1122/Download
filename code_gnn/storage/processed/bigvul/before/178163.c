 static void si_conn_send(struct connection *conn)
 {
 	struct stream_interface *si = conn->owner;
 	struct channel *chn = si->ob;
 	int ret;
 
         if (chn->pipe && conn->xprt->snd_pipe) {
                 ret = conn->xprt->snd_pipe(conn, chn->pipe);
                 if (ret > 0)
                       chn->flags |= CF_WRITE_PARTIAL;
//                        chn->flags |= CF_WRITE_PARTIAL | CF_WROTE_DATA;
  
                 if (!chn->pipe->data) {
                         put_pipe(chn->pipe);
 			chn->pipe = NULL;
 		}
 
 		if (conn->flags & CO_FL_ERROR)
 			return;
 	}
 
 	 
 	if (!chn->buf->o)
 		return;
 
 	 
 	if (!(conn->flags & (CO_FL_ERROR | CO_FL_SOCK_WR_SH | CO_FL_DATA_WR_SH | CO_FL_WAIT_DATA | CO_FL_HANDSHAKE))) {
 		 
 		unsigned int send_flag = 0;
 
 		if ((!(chn->flags & (CF_NEVER_WAIT|CF_SEND_DONTWAIT)) &&
 		     ((chn->to_forward && chn->to_forward != CHN_INFINITE_FORWARD) ||
 		      (chn->flags & CF_EXPECT_MORE))) ||
 		    ((chn->flags & (CF_SHUTW|CF_SHUTW_NOW)) == CF_SHUTW_NOW))
 			send_flag |= CO_SFL_MSG_MORE;
 
 		if (chn->flags & CF_STREAMER)
 			send_flag |= CO_SFL_STREAMER;
  
                 ret = conn->xprt->snd_buf(conn, chn->buf, send_flag);
                 if (ret > 0) {
                       chn->flags |= CF_WRITE_PARTIAL;
//                        chn->flags |= CF_WRITE_PARTIAL | CF_WROTE_DATA;
  
                         if (!chn->buf->o) {
                                  
 				chn->flags &= ~(CF_EXPECT_MORE | CF_SEND_DONTWAIT);
 			}
 
 			 
 		}
 	}
 
 	return;
 }
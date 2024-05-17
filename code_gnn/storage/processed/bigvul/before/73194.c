socks_request_set_socks5_error(socks_request_t *req,
                  socks5_reply_status_t reason)
{
   req->replylen = 10;
   memset(req->reply,0,10);

   req->reply[0] = 0x05;    
   req->reply[1] = reason;  
   req->reply[3] = 0x01;    
}

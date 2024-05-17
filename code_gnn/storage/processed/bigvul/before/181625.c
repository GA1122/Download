 void processInputBuffer(client *c) {
     server.current_client = c;
      
     while(sdslen(c->querybuf)) {
          
         if (!(c->flags & CLIENT_SLAVE) && clientsArePaused()) break;
 
          
         if (c->flags & CLIENT_BLOCKED) break;
  
           
        if (c->flags & CLIENT_CLOSE_AFTER_REPLY) break;
//         if (c->flags & (CLIENT_CLOSE_AFTER_REPLY|CLIENT_CLOSE_ASAP)) break;
  
           
          if (!c->reqtype) {
             if (c->querybuf[0] == '*') {
                 c->reqtype = PROTO_REQ_MULTIBULK;
             } else {
                 c->reqtype = PROTO_REQ_INLINE;
             }
         }
 
         if (c->reqtype == PROTO_REQ_INLINE) {
             if (processInlineBuffer(c) != C_OK) break;
         } else if (c->reqtype == PROTO_REQ_MULTIBULK) {
             if (processMultibulkBuffer(c) != C_OK) break;
         } else {
             serverPanic("Unknown request type");
         }
 
          
         if (c->argc == 0) {
             resetClient(c);
         } else {
              
             if (processCommand(c) == C_OK)
                 resetClient(c);
              
             if (server.current_client == NULL) break;
         }
     }
     server.current_client = NULL;
 }
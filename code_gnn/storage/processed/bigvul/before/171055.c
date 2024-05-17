void* RilSapSocket::processRequestsLoop(void) {
    RLOGI("UIM_SOCKET:Request loop started");

 while(true) {
 SapSocketRequest *req = dispatchQueue.dequeue();

        RLOGI("New request from the dispatch Queue");

 if (req != NULL) {
            dispatchRequest(req->curr);
            free(req);
 } else {
            RLOGE("Fetched null buffer from queue!");
 }
 }
 return NULL;
}

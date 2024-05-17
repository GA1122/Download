rsRetVal writeZMQ(uchar* msg, instanceData* pData) {
	DEFiRet;

     
    if(NULL == pData->socket)
		CHKiRet(initZMQ(pData));
    
     
    int result = zstr_send(pData->socket, (char*)msg);
    
     
    if (result == -1) {
        errmsg.LogError(0, NO_ERRCODE, "omzmq3: send of %s failed: %s", msg, zmq_strerror(errno));
        ABORT_FINALIZE(RS_RET_ERR);
    }
 finalize_it:
    RETiRet;
}
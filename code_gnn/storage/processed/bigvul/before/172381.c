omx_video::~omx_video()
{
    DEBUG_PRINT_HIGH("~omx_video(): Inside Destructor()");
 if (m_pipe_in >= 0) close(m_pipe_in);
 if (m_pipe_out >= 0) close(m_pipe_out);
    DEBUG_PRINT_HIGH("omx_video: Waiting on Msg Thread exit");
 if (msg_thread_created)
        pthread_join(msg_thread_id,NULL);
    DEBUG_PRINT_HIGH("omx_video: Waiting on Async Thread exit");
  
#ifndef _MSM8974_
 if (async_thread_created)
        pthread_join(async_thread_id,NULL);
#endif
    pthread_mutex_destroy(&m_lock);
    sem_destroy(&m_cmd_lock);
    DEBUG_PRINT_HIGH("m_etb_count = %" PRIu64 ", m_fbd_count = %" PRIu64, m_etb_count,
            m_fbd_count);
    DEBUG_PRINT_HIGH("omx_video: Destructor exit");
    DEBUG_PRINT_HIGH("Exiting OMX Video Encoder ...");
}

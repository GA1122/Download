 static void a2dp_open_ctrl_path(struct a2dp_stream_common *common)
 {
  int i;
 
   
  for (i = 0; i < CTRL_CHAN_RETRY_COUNT; i++)
  {
   
  if ((common->ctrl_fd = skt_connect(A2DP_CTRL_PATH, common->buffer_sz)) > 0)
  {
   
  if (check_a2dp_ready(common) == 0)
 
                  break;
  
              ERROR("error : a2dp not ready, wait 250 ms and retry");
            usleep(250000);
//             TEMP_FAILURE_RETRY(usleep(250000));
              skt_disconnect(common->ctrl_fd);
              common->ctrl_fd = AUDIO_SKT_DISCONNECTED;
          }
  
           
        usleep(250000);
//         TEMP_FAILURE_RETRY(usleep(250000));
      }
  }
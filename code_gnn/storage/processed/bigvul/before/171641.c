static int check_a2dp_ready(struct a2dp_stream_common *common)
{
 if (a2dp_command(common, A2DP_CTRL_CMD_CHECK_READY) < 0)
 {
        ERROR("check a2dp ready failed");
 return -1;
 }
 return 0;
}

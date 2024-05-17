bool btif_av_is_sink_enabled(void) {
 return (bt_av_sink_callbacks != NULL) ? true : false;
}

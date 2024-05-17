void _WM_do_meta_marker(struct _mdi *mdi, struct _event_data *data) {
 
#ifdef DEBUG_MIDI
    uint8_t ch = data->channel;
    MIDI_EVENT_SDEBUG(__FUNCTION__, ch, data->data.string);
#else
    UNUSED(data);
#endif
    UNUSED(mdi);
    return;
}

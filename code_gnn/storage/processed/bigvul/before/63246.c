void _WM_do_meta_lyric(struct _mdi *mdi, struct _event_data *data) {
 
#ifdef DEBUG_MIDI
    uint8_t ch = data->channel;
    MIDI_EVENT_SDEBUG(__FUNCTION__, ch, data->data.string);
#endif
    if (!(mdi->extra_info.mixer_options & WM_MO_TEXTASLYRIC)) {
        mdi->lyric = data->data.string;
    }
    return;
}

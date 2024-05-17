_WM_Event2Midi(struct _mdi *mdi, uint8_t **out, uint32_t *outsize) {
    uint32_t out_ofs = 0;
    uint8_t running_event = 0;
    uint32_t divisions = 96;
    uint32_t tempo = 500000;
    float samples_per_tick = 0.0;
    uint32_t value = 0;
    float value_f = 0.0;
    struct _event *event = mdi->events;
    uint32_t track_size = 0;
    uint32_t track_start = 0;
    uint32_t track_count = 0;

    if (!mdi->event_count) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_CONVERT, "(No events to convert)", 0);
        return -1;
    }

    samples_per_tick = _WM_GetSamplesPerTick(divisions, tempo);

     
    (*out) = malloc (sizeof(uint8_t) * (mdi->event_count * 12));

     
    (*out)[0] = 'M';
    (*out)[1] = 'T';
    (*out)[2] = 'h';
    (*out)[3] = 'd';
    (*out)[4] = 0x00;
    (*out)[5] = 0x00;
    (*out)[6] = 0x00;
    (*out)[7] = 0x06;
    if ((!(_WM_MixerOptions & WM_MO_SAVEASTYPE0)) && (mdi->is_type2)) {
         
        (*out)[8] = 0x00;
        (*out)[9] = 0x02;
    } else {
         
        (*out)[8] = 0x00;
        (*out)[9] = 0x00;
    }
     
     
     
    (*out)[14] = 'M';
    (*out)[15] = 'T';
    (*out)[16] = 'r';
    (*out)[17] = 'k';
     
    out_ofs = 22;
    track_start = out_ofs;
    track_count++;

    do {
         
        if (event->do_event == _WM_do_midi_divisions) {
            divisions = event->event_data.data.value;
            (*out)[12] = (divisions >> 8) & 0xff;
            (*out)[13] = divisions & 0xff;
            samples_per_tick = _WM_GetSamplesPerTick(divisions, tempo);
        } else if (event->do_event == _WM_do_note_off) {
            if (running_event != (0x80 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0x80 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = (event->event_data.data.value >> 8) & 0xff;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_note_on) {
            if (running_event != (0x90 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0x90 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = (event->event_data.data.value >> 8) & 0xff;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_aftertouch) {
            if (running_event != (0xa0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xa0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = (event->event_data.data.value >> 8) & 0xff;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_bank_select) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 0;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_data_entry_course) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 6;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_volume) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 7;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_balance) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 8;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_pan) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 10;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_expression) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 11;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_data_entry_fine) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 38;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_hold) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 64;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_data_increment) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 96;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_data_decrement) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 97;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_non_registered_param_fine) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 98;
            (*out)[out_ofs++] = event->event_data.data.value & 0x7f;
        } else if (event->do_event == _WM_do_control_non_registered_param_course) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 99;
            (*out)[out_ofs++] = (event->event_data.data.value >> 7) & 0x7f;
        } else if (event->do_event == _WM_do_control_registered_param_fine) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 100;
            (*out)[out_ofs++] = event->event_data.data.value & 0x7f;
        } else if (event->do_event == _WM_do_control_registered_param_course) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 101;
            (*out)[out_ofs++] = (event->event_data.data.value >> 7) & 0x7f;
        } else if (event->do_event == _WM_do_control_channel_sound_off) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 120;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_controllers_off) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 121;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_channel_notes_off) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = 123;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_control_dummy) {
            if (running_event != (0xb0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xb0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = (event->event_data.data.value >> 8) & 0xff;
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_patch) {
            if (running_event != (0xc0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xc0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_channel_pressure) {
            if (running_event != (0xd0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xd0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = event->event_data.data.value & 0xff;
        } else if (event->do_event == _WM_do_pitch) {
            if (running_event != (0xe0 | event->event_data.channel)) {
                (*out)[out_ofs++] = 0xe0 | event->event_data.channel;
                running_event = (*out)[out_ofs - 1];
            }
            (*out)[out_ofs++] = event->event_data.data.value & 0x7f;
            (*out)[out_ofs++] = (event->event_data.data.value >> 7) & 0x7f;
        } else if (event->do_event == _WM_do_sysex_roland_drum_track) {
            uint8_t foo[] = {0xf0, 0x09, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x15, 0x00, 0xf7};
            uint8_t foo_ch = event->event_data.channel;
            if (foo_ch == 9) {
                foo_ch = 0;
            } else if (foo_ch < 9) {
                foo_ch++;
            }
            foo[7] = 0x10 | foo_ch;
            foo[9] = event->event_data.data.value;
            memcpy(&((*out)[out_ofs]),foo,11);
            out_ofs += 11;
            running_event = 0;
        } else if (event->do_event == _WM_do_sysex_gm_reset) {
            uint8_t foo[] = {0xf0, 0x05, 0x7e, 0x7f, 0x09, 0x01, 0xf7};
            memcpy(&((*out)[out_ofs]),foo,7);
            out_ofs += 7;
            running_event = 0;
        } else if (event->do_event == _WM_do_sysex_roland_reset) {
            uint8_t foo[] = {0xf0, 0x0a, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7f, 0x00, 0x41, 0xf7};
            memcpy(&((*out)[out_ofs]),foo,12);
            out_ofs += 12;
            running_event = 0;
        } else if (event->do_event == _WM_do_sysex_yamaha_reset) {
            uint8_t foo[] = {0xf0, 0x08, 0x43, 0x10, 0x4c, 0x00, 0x00, 0x7e, 0x00, 0xf7};
            memcpy(&((*out)[out_ofs]),foo,10);
            out_ofs += 10;
            running_event = 0;
        } else if (event->do_event == _WM_do_meta_endoftrack) {
            if ((!(_WM_MixerOptions & WM_MO_SAVEASTYPE0)) && (mdi->is_type2)) {
                 
                (*out)[out_ofs++] = 0xff;
                (*out)[out_ofs++] = 0x2f;
                (*out)[out_ofs++] = 0x00;
                track_size = out_ofs - track_start;
                (*out)[track_start - 4] = (track_size >> 24) & 0xff;
                (*out)[track_start - 3] = (track_size >> 16) & 0xff;
                (*out)[track_start - 2] = (track_size >> 8) & 0xff;
                (*out)[track_start - 1] = track_size & 0xff;

                if (event[1].do_event != NULL) {
                    (*out)[out_ofs++] = 'M';
                    (*out)[out_ofs++] = 'T';
                    (*out)[out_ofs++] = 'r';
                    (*out)[out_ofs++] = 'k';
                    track_count++;
                    out_ofs += 4;
                    track_start = out_ofs;

                     
                    (*out)[out_ofs++] = 0;

                    running_event = 0;
                }
            }
            goto NEXT_EVENT;
        } else if (event->do_event == _WM_do_meta_tempo) {
            tempo = event->event_data.data.value & 0xffffff;

            samples_per_tick = _WM_GetSamplesPerTick(divisions, tempo);


            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x51;
            (*out)[out_ofs++] = 0x03;
            (*out)[out_ofs++] = (tempo & 0xff0000) >> 16;
            (*out)[out_ofs++] = (tempo & 0xff00) >> 8;
            (*out)[out_ofs++] = (tempo & 0xff);
        } else if (event->do_event == _WM_do_meta_timesignature) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x58;
            (*out)[out_ofs++] = 0x04;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff000000) >> 24;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff0000) >> 16;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff00) >> 8;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);
        } else if (event->do_event == _WM_do_meta_keysignature) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x59;
            (*out)[out_ofs++] = 0x02;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff00) >> 8;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);
        } else if (event->do_event == _WM_do_meta_sequenceno) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x00;
            (*out)[out_ofs++] = 0x02;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff00) >> 8;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);
        } else if (event->do_event == _WM_do_meta_channelprefix) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x20;
            (*out)[out_ofs++] = 0x01;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);
        } else if (event->do_event == _WM_do_meta_portprefix) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x21;
            (*out)[out_ofs++] = 0x01;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);
        } else if (event->do_event == _WM_do_meta_smpteoffset) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x54;
            (*out)[out_ofs++] = 0x05;
             
            (*out)[out_ofs++] = (event->event_data.channel & 0xff);
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff000000) >> 24;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff0000) >> 16;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff00) >> 8;
            (*out)[out_ofs++] = (event->event_data.data.value & 0xff);

        } else if (event->do_event == _WM_do_meta_text) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x01;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_copyright) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x02;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_trackname) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x03;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_instrumentname) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x04;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_lyric) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x05;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_marker) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x06;

            goto _WRITE_TEXT;

        } else if (event->do_event == _WM_do_meta_cuepoint) {
            (*out)[out_ofs++] = 0xff;
            (*out)[out_ofs++] = 0x07;

            _WRITE_TEXT:
            value = strlen(event->event_data.data.string);
            if (value > 0x0fffffff)
                (*out)[out_ofs++] = (((value >> 28) &0x7f) | 0x80);
            if (value > 0x1fffff)
                (*out)[out_ofs++] = (((value >> 21) &0x7f) | 0x80);
            if (value > 0x3fff)
                (*out)[out_ofs++] = (((value >> 14) & 0x7f) | 0x80);
            if (value > 0x7f)
                (*out)[out_ofs++] = (((value >> 7) & 0x7f) | 0x80);
            (*out)[out_ofs++] = (value & 0x7f);

            memcpy(&(*out)[out_ofs], event->event_data.data.string, value);
            out_ofs += value;

        } else {
            fprintf(stderr,"Unknown Event %.2x %.4x\n",event->event_data.channel, event->event_data.data.value);
            event++;
            continue;
        }

        value_f = (float)event->samples_to_next / samples_per_tick;
        value = (uint32_t)(value_f + 0.5);


        if (value > 0x0fffffff)
            (*out)[out_ofs++] = (((value >> 28) &0x7f) | 0x80);
        if (value > 0x1fffff)
            (*out)[out_ofs++] = (((value >> 21) &0x7f) | 0x80);
        if (value > 0x3fff)
            (*out)[out_ofs++] = (((value >> 14) & 0x7f) | 0x80);
        if (value > 0x7f)
            (*out)[out_ofs++] = (((value >> 7) & 0x7f) | 0x80);
        (*out)[out_ofs++] = (value & 0x7f);
    NEXT_EVENT:
        event++;
    } while (event->do_event != NULL);

    if ((_WM_MixerOptions & WM_MO_SAVEASTYPE0) || (!mdi->is_type2)) {
         
        (*out)[out_ofs++] = 0xff;
        (*out)[out_ofs++] = 0x2f;
        (*out)[out_ofs++] = 0x00;

         
        track_size = out_ofs - track_start;
        (*out)[track_start - 4] = (track_size >> 24) & 0xff;
        (*out)[track_start - 3] = (track_size >> 16) & 0xff;
        (*out)[track_start - 2] = (track_size >> 8) & 0xff;
        (*out)[track_start - 1] = track_size & 0xff;
    }
     
    (*out)[10] = (track_count >> 8) & 0xff;
    (*out)[11] = track_count & 0xff;

    (*out) = realloc((*out), out_ofs);
    (*outsize) = out_ofs;

    return 0;
}
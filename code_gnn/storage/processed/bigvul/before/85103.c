WM_SYMBOL int WildMidi_FastSeek(midi * handle, unsigned long int *sample_pos) {
    struct _mdi *mdi;
    struct _event *event;
    struct _note *note_data;

    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (-1);
    }
    if (sample_pos == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL seek position pointer)", 0);
        return (-1);
    }

    mdi = (struct _mdi *) handle;
    _WM_Lock(&mdi->lock);
    event = mdi->current_event;

     
    if (*sample_pos > mdi->extra_info.approx_total_samples) {
         
        *sample_pos = mdi->extra_info.approx_total_samples;
    }

     
    if (*sample_pos == mdi->extra_info.approx_total_samples) {
         
        _WM_Unlock(&mdi->lock);
        return (0);
    }

     
    if (mdi->extra_info.current_sample > *sample_pos) {
         
        event = mdi->events;
        _WM_ResetToStart(handle);
        mdi->extra_info.current_sample = 0;
        mdi->samples_to_mix = 0;
    }

    if ((mdi->extra_info.current_sample + mdi->samples_to_mix) > *sample_pos) {
        mdi->samples_to_mix = (mdi->extra_info.current_sample + mdi->samples_to_mix) - *sample_pos;
        mdi->extra_info.current_sample = *sample_pos;
    } else {
        mdi->extra_info.current_sample += mdi->samples_to_mix;
        mdi->samples_to_mix = 0;
        while ((!mdi->samples_to_mix) && (event->do_event)) {
            event->do_event(mdi, &event->event_data);
            mdi->samples_to_mix = event->samples_to_next;
                
            if ((mdi->extra_info.current_sample + mdi->samples_to_mix) > *sample_pos) {
                mdi->samples_to_mix = (mdi->extra_info.current_sample + mdi->samples_to_mix) - *sample_pos;
                mdi->extra_info.current_sample = *sample_pos;
            } else {
                mdi->extra_info.current_sample += mdi->samples_to_mix;
                mdi->samples_to_mix = 0;
            }
            event++;
        }
        mdi->current_event = event;
    }

     
    note_data = mdi->note;
    if (note_data) {
        do {
            note_data->active = 0;
            if (note_data->replay) {
                note_data->replay = NULL;
            }
            note_data = note_data->next;
        } while (note_data);
    }
    mdi->note = NULL;

     
    _WM_reset_reverb(mdi->reverb);

    _WM_Unlock(&mdi->lock);
    return (0);
}

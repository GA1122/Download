  _WM_ParseNewHmi(uint8_t *hmi_data, uint32_t hmi_size) {
      uint32_t hmi_tmp = 0;
      uint8_t *hmi_base = hmi_data;
//     uint32_t data_siz;
      uint16_t hmi_bpm = 0;
      uint16_t hmi_division = 0;
  
      uint32_t *hmi_track_offset = NULL;
      uint32_t i = 0;
     uint32_t j = 0;
     uint8_t *hmi_addr = NULL;
     uint32_t *hmi_track_header_length = NULL;
     struct _mdi *hmi_mdi = NULL;
     uint32_t tempo_f = 5000000.0;
     uint32_t *hmi_track_end = NULL;
     uint8_t hmi_tracks_ended = 0;
     uint8_t *hmi_running_event = NULL;
     uint32_t setup_ret = 0;
     uint32_t *hmi_delta = NULL;
 
     uint32_t smallest_delta = 0;
     uint32_t subtract_delta = 0;
 
     uint32_t sample_count = 0;
     float sample_count_f = 0;
     float sample_remainder = 0;
 
     float samples_per_delta_f = 0.0;
 
     struct _note {
         uint32_t length;
          uint8_t channel;
      } *note;
  
    UNUSED(hmi_size);
  
      if (memcmp(hmi_data, "HMI-MIDISONG061595", 18)) {
          _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, NULL, 0);
         return NULL;
     }
 
     hmi_bpm = hmi_data[212];
 
     hmi_division = 60;
 
     hmi_track_cnt = hmi_data[228];
 
     hmi_mdi = _WM_initMDI();
 
     _WM_midi_setup_divisions(hmi_mdi, hmi_division);
 
     if ((_WM_MixerOptions & WM_MO_ROUNDTEMPO)) {
         tempo_f = (float) (60000000 / hmi_bpm) + 0.5f;
     } else {
         tempo_f = (float) (60000000 / hmi_bpm);
     }
     samples_per_delta_f = _WM_GetSamplesPerTick(hmi_division, (uint32_t)tempo_f);
 
     _WM_midi_setup_tempo(hmi_mdi, (uint32_t)tempo_f);
 
     hmi_track_offset = (uint32_t *)malloc(sizeof(uint32_t) * hmi_track_cnt);
     hmi_track_header_length = malloc(sizeof(uint32_t) * hmi_track_cnt);
     hmi_track_end = malloc(sizeof(uint32_t) * hmi_track_cnt);
     hmi_delta = malloc(sizeof(uint32_t) * hmi_track_cnt);
     note = malloc(sizeof(struct _note) * 128 * hmi_track_cnt);
     hmi_running_event = malloc(sizeof(uint8_t) * 128 * hmi_track_cnt);
 
     hmi_data += 370;
 
     smallest_delta = 0xffffffff;
 
     if (hmi_size < (370 + (hmi_track_cnt * 17))) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
         goto _hmi_end;
     }
 
     hmi_track_offset[0] = *hmi_data;  
 
     for (i = 0; i < hmi_track_cnt; i++) {
         hmi_track_offset[i] = *hmi_data++;
         hmi_track_offset[i] += (*hmi_data++ << 8);
         hmi_track_offset[i] += (*hmi_data++ << 16);
         hmi_track_offset[i] += (*hmi_data++ << 24);
 
         if (hmi_size < (hmi_track_offset[i] + 0x5a + 4)) {
             _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
             goto _hmi_end;
         }
 
         hmi_addr = hmi_base + hmi_track_offset[i];
 
         if (memcmp(hmi_addr, "HMI-MIDITRACK", 13)) {
             _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, NULL, 0);
             goto _hmi_end;
         }
 
         hmi_track_header_length[i] = hmi_addr[0x57];
         hmi_track_header_length[i] += (hmi_addr[0x58] << 8);
         hmi_track_header_length[i] += (hmi_addr[0x59] << 16);
         hmi_track_header_length[i] += (hmi_addr[0x5a] << 24);
 
         hmi_addr += hmi_track_header_length[i];
         hmi_track_offset[i] += hmi_track_header_length[i];
 
         hmi_delta[i] = 0;
         if (*hmi_addr > 0x7f) {
             do {
                 hmi_delta[i] = (hmi_delta[i] << 7) + (*hmi_addr & 0x7f);
                 hmi_addr++;
                 hmi_track_offset[i]++;
             } while (*hmi_addr > 0x7f);
         }
         hmi_delta[i] = (hmi_delta[i] << 7) + (*hmi_addr & 0x7f);
         hmi_track_offset[i]++;
         hmi_addr++;
 
         if (hmi_delta[i] < smallest_delta) {
             smallest_delta = hmi_delta[i];
         }
 
         hmi_track_end[i] = 0;
         hmi_running_event[i] = 0;
 
         for (j = 0; j < 128; j++) {
             hmi_tmp = (128 * i) + j;
             note[hmi_tmp].length = 0;
             note[hmi_tmp].channel = 0;
         }
     }
 
     subtract_delta = smallest_delta;
     sample_count_f= (((float) smallest_delta * samples_per_delta_f) + sample_remainder);
 
     sample_count = (uint32_t) sample_count_f;
     sample_remainder = sample_count_f - (float) sample_count;
 
     hmi_mdi->events[hmi_mdi->event_count - 1].samples_to_next += sample_count;
     hmi_mdi->extra_info.approx_total_samples += sample_count;
 
     while (hmi_tracks_ended < hmi_track_cnt) {
         smallest_delta = 0;
         for (i = 0; i < hmi_track_cnt; i++) {
             if (hmi_track_end[i]) continue;
 
             for (j = 0; j < 128; j++) {
                 hmi_tmp = (128 * i) + j;
                 if (note[hmi_tmp].length) {
                     note[hmi_tmp].length -= subtract_delta;
                     if (note[hmi_tmp].length) {
                         if ((!smallest_delta) || (smallest_delta > note[hmi_tmp].length)) {
                             smallest_delta = note[hmi_tmp].length;
                         }
                     } else {
                         _WM_midi_setup_noteoff(hmi_mdi, note[hmi_tmp].channel, j, 0);
                     }
                 }
             }
 
             if (hmi_delta[i]) {
                 hmi_delta[i] -= subtract_delta;
                 if (hmi_delta[i]) {
                     if ((!smallest_delta) || (smallest_delta > hmi_delta[i])) {
                         smallest_delta = hmi_delta[i];
                     }
                     continue;
                 }
             }
 
              do {
                  hmi_data = hmi_base + hmi_track_offset[i];
                  hmi_delta[i] = 0;
//                 if (hmi_track_offset[i] >= hmi_size) {
//                     _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
//                     goto _hmi_end;
//                 }
//                 data_siz = hmi_size - hmi_track_offset[i];
  
                  if (hmi_data[0] == 0xfe) {
                      if (hmi_data[1] == 0x10) {
                          hmi_tmp = (hmi_data[4] + 5);
                          hmi_data += hmi_tmp;
                          hmi_track_offset[i] += hmi_tmp;
//                         hmi_tmp += 4;
                      } else if (hmi_data[1] == 0x15) {
                          hmi_data += 4;
                          hmi_track_offset[i] += 4;
//                         hmi_tmp = 8;
//                     } else {
//                         hmi_tmp = 4;
                      }
                      hmi_data += 4;
                      hmi_track_offset[i] += 4;
//                     if (hmi_tmp > data_siz) {
//                         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
//                         goto _hmi_end;
//                     }
//                     data_siz -= hmi_tmp;
                  } else {
                    if ((setup_ret = _WM_SetupMidiEvent(hmi_mdi,hmi_data,hmi_running_event[i])) == 0) {
//                     if ((setup_ret = _WM_SetupMidiEvent(hmi_mdi,hmi_data,data_siz,hmi_running_event[i])) == 0) {
                          goto _hmi_end;
                      }
                      if ((hmi_data[0] == 0xff) && (hmi_data[1] == 0x2f) && (hmi_data[2] == 0x00)) {
                         hmi_track_end[i] = 1;
                         hmi_tracks_ended++;
                         for(j = 0; j < 128; j++) {
                             hmi_tmp = (128 * i) + j;
                             if (note[hmi_tmp].length) {
                                 _WM_midi_setup_noteoff(hmi_mdi, note[hmi_tmp].channel, j, 0);
                                 note[hmi_tmp].length = 0;
                             }
                         }
                         goto _hmi_next_track;
                     }
                     if ((*hmi_data == 0xF0) || (*hmi_data == 0xF7)) {
                         hmi_running_event[i] = 0;
                     } else if (*hmi_data < 0xF0) {
                         if (*hmi_data >= 0x80) {
                             hmi_running_event[i] = *hmi_data;
                         }
                     }
                     if ((hmi_running_event[i] & 0xf0) == 0x90) {
                         if (*hmi_data > 127) {
                             hmi_tmp = hmi_data[1];
                         } else {
                             hmi_tmp = *hmi_data;
                         }
                         hmi_tmp += (i * 128);
 
                         note[hmi_tmp].channel = hmi_running_event[i] & 0xf;
  
                          hmi_data += setup_ret;
                          hmi_track_offset[i] += setup_ret;
//                         data_siz -= setup_ret;
  
                          note[hmi_tmp].length = 0;
                        if (*hmi_data > 0x7f) {
//                         if (data_siz && *hmi_data > 0x7f) {
                              do {
//                                 if (!data_siz) break;
                                  note[hmi_tmp].length = (note[hmi_tmp].length << 7) | (*hmi_data & 0x7F);
                                  hmi_data++;
//                                 data_siz--;
                                  hmi_track_offset[i]++;
                              } while (*hmi_data > 0x7F);
                          }
//                         if (!data_siz) {
//                             _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
//                             goto _hmi_end;
//                         }
                          note[hmi_tmp].length = (note[hmi_tmp].length << 7) | (*hmi_data & 0x7F);
                          hmi_data++;
//                         data_siz--;
                          hmi_track_offset[i]++;
  
                          if (note[hmi_tmp].length) {
                             if ((!smallest_delta) || (smallest_delta > note[hmi_tmp].length)) {
                                 smallest_delta = note[hmi_tmp].length;
                             }
                         } else {
                             _WM_midi_setup_noteoff(hmi_mdi, note[hmi_tmp].channel, j, 0);
                         }
 
                      } else {
                          hmi_data += setup_ret;
                          hmi_track_offset[i] += setup_ret;
//                         data_siz -= setup_ret;
                      }
                  }
  
                if (*hmi_data > 0x7f) {
//                 if (data_siz && *hmi_data > 0x7f) {
                      do {
//                         if (!data_siz) break;
                          hmi_delta[i] = (hmi_delta[i] << 7) | (*hmi_data & 0x7F);
                          hmi_data++;
//                         data_siz--;
                          hmi_track_offset[i]++;
                      } while (*hmi_data > 0x7F);
                  }
//                 if (!data_siz) {
//                     _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_HMI, "file too short", 0);
//                     goto _hmi_end;
//                 }
                  hmi_delta[i] = (hmi_delta[i] << 7) | (*hmi_data & 0x7F);
                  hmi_data++;
//                 data_siz--;
                  hmi_track_offset[i]++;
              } while (!hmi_delta[i]);
              if ((!smallest_delta) || (smallest_delta > hmi_delta[i])) {
                 smallest_delta = hmi_delta[i];
             }
 
         _hmi_next_track:
             hmi_tmp = 0;
             UNUSED(hmi_tmp);
         }
 
         subtract_delta = smallest_delta;
         sample_count_f= (((float) smallest_delta * samples_per_delta_f) + sample_remainder);
 
         sample_count = (uint32_t) sample_count_f;
         sample_remainder = sample_count_f - (float) sample_count;
 
         hmi_mdi->events[hmi_mdi->event_count - 1].samples_to_next += sample_count;
         hmi_mdi->extra_info.approx_total_samples += sample_count;
     }
 
     if ((hmi_mdi->reverb = _WM_init_reverb(_WM_SampleRate, _WM_reverb_room_width, _WM_reverb_room_length, _WM_reverb_listen_posx, _WM_reverb_listen_posy)) == NULL) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, "to init reverb", 0);
         goto _hmi_end;
     }
 
     hmi_mdi->extra_info.current_sample = 0;
     hmi_mdi->current_event = &hmi_mdi->events[0];
     hmi_mdi->samples_to_mix = 0;
     hmi_mdi->note = NULL;
 
     _WM_ResetToStart(hmi_mdi);
 
 _hmi_end:
     free(hmi_track_offset);
     free(hmi_track_header_length);
     free(hmi_track_end);
     free(hmi_delta);
     free(note);
     free(hmi_running_event);
 
     if (hmi_mdi->reverb) return (hmi_mdi);
     _WM_freeMDI(hmi_mdi);
     return 0;
 }
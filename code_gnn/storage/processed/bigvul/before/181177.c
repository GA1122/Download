 _WM_ParseNewMus(uint8_t *mus_data, uint32_t mus_size) {
     uint8_t mus_hdr[] = { 'M', 'U', 'S', 0x1A };
     uint32_t mus_song_ofs = 0;
     uint32_t mus_song_len = 0;
     uint16_t mus_ch_cnt1 = 0;
     uint16_t mus_ch_cnt2 = 0;
     uint16_t mus_no_instr = 0;
     uint32_t mus_data_ofs = 0;
     uint16_t * mus_mid_instr = NULL;
     uint16_t mus_instr_cnt = 0;
     struct _mdi *mus_mdi;
     uint32_t mus_divisions = 60;
      float tempo_f = 0.0;
      uint16_t mus_freq = 0;
      float samples_per_tick_f = 0.0;
    uint8_t mus_event[] = { 0, 0, 0, 0 };
// #define MUS_SZ 4
//     uint8_t mus_event[MUS_SZ] = { 0, 0, 0, 0 };
      uint8_t mus_event_size = 0;
      uint8_t mus_prev_vol[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      uint32_t setup_ret = 0;
     uint32_t mus_ticks = 0;
     uint32_t sample_count = 0;
     float sample_count_f = 0.0;
     float sample_remainder = 0.0;
     uint16_t pitchbend_tmp = 0;
 
     if (mus_size < 17) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_MUS, "File too short", 0);
         return NULL;
     }
 
     if (memcmp(mus_data, mus_hdr, 4)) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_MUS, NULL, 0);
         return NULL;
     }
 
     mus_song_len = (mus_data[5] << 8) | mus_data[4];
     mus_song_ofs = (mus_data[7] << 8) | mus_data[6];
 
     mus_ch_cnt1 = (mus_data[9] << 8) | mus_data[8];
     mus_ch_cnt2 = (mus_data[11] << 8) | mus_data[10];
 
     UNUSED(mus_ch_cnt1);
     UNUSED(mus_ch_cnt2);
 
     mus_no_instr = (mus_data[13] << 8) | mus_data[12];
 
     mus_data_ofs = 16;
 
     if (mus_size < (mus_data_ofs + (mus_no_instr << 1) + mus_song_len)) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_MUS, "File too short", 0);
         return NULL;
     }
 
     mus_mid_instr = malloc(mus_no_instr * sizeof(uint16_t));
     for (mus_instr_cnt = 0; mus_instr_cnt < mus_no_instr; mus_instr_cnt++) {
         mus_mid_instr[mus_instr_cnt] = (mus_data[mus_data_ofs + 1] << 8) | mus_data[mus_data_ofs];
         mus_data_ofs += 2;
     }
 
     mus_data_ofs = mus_song_ofs;
 
     mus_freq = _cvt_get_option(WM_CO_FREQUENCY);
     if (mus_freq == 0) mus_freq = 140;
 
     if ((_WM_MixerOptions & WM_MO_ROUNDTEMPO)) {
         tempo_f = (float) (60000000 / mus_freq) + 0.5f;
     } else {
         tempo_f = (float) (60000000 / mus_freq);
     }
 
     samples_per_tick_f = _WM_GetSamplesPerTick(mus_divisions, (uint32_t)tempo_f);
 
     mus_mdi = _WM_initMDI();
     _WM_midi_setup_divisions(mus_mdi, mus_divisions);
     _WM_midi_setup_tempo(mus_mdi, (uint32_t)tempo_f);
 
     do {
     _mus_build_event:
 #if 1
         MUS_EVENT_DEBUG("Before", mus_data[mus_data_ofs], 0);
 
         if ((mus_data[mus_data_ofs] & 0x0f) == 0x0f) {
             mus_data[mus_data_ofs] = (mus_data[mus_data_ofs] & 0xf0) | 0x09;
         } else if ((mus_data[mus_data_ofs] & 0x0f) == 0x09) {
             mus_data[mus_data_ofs] = (mus_data[mus_data_ofs] & 0xf0) | 0x0f;
         }
         MUS_EVENT_DEBUG("After", mus_data[mus_data_ofs], 0);
 #endif
         switch ((mus_data[mus_data_ofs] >> 4) & 0x07) {
             case 0:  
                 mus_event_size = 2;
                 mus_event[0] = 0x80 | (mus_data[mus_data_ofs] & 0x0f);
                 mus_event[1] = mus_data[mus_data_ofs + 1];
                 mus_event[2] = 0;
                 mus_event[3] = 0;
                 break;
             case 1:  
                 if (mus_data[mus_data_ofs + 1] & 0x80) {
                     mus_event_size = 3;
                     mus_event[0] = 0x90 | (mus_data[mus_data_ofs] & 0x0f);
                     mus_event[1] = mus_data[mus_data_ofs + 1] & 0x7f;
                     mus_event[2] = mus_data[mus_data_ofs + 2];
                     mus_event[3] = 0;
                     mus_prev_vol[mus_data[mus_data_ofs] & 0x0f] = mus_event[2];
                 } else {
                     mus_event_size = 2;
                     mus_event[0] = 0x90 | (mus_data[mus_data_ofs] & 0x0f);
                     mus_event[1] = mus_data[mus_data_ofs + 1];
                     mus_event[2] = mus_prev_vol[mus_data[mus_data_ofs] & 0x0f];
                     mus_event[3] = 0;
                 }
                 break;
             case 2:  
                 mus_event_size = 2;
                 mus_event[0] = 0xe0 | (mus_data[mus_data_ofs] & 0x0f);
 
                 pitchbend_tmp = mus_data[mus_data_ofs + 1] << 6;
                 mus_event[1] = pitchbend_tmp & 0x7f;
                 mus_event[2] = (pitchbend_tmp >> 7) & 0x7f;
                 mus_event[3] = 0;
                 break;
             case 3:
                 mus_event_size = 2;
                 switch (mus_data[mus_data_ofs + 1]) {
                     case 10:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 120;
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     case 11:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 123;
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     case 12:  
                          
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 126;
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     case 13:  
                          
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 127;
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     case 14:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 121;
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     default:  
                         goto _mus_next_data;
                 }
                 break;
             case 4:
                 mus_event_size = 3;
                 switch (mus_data[mus_data_ofs + 1]) {
                     case 0:  
                          
                         mus_event[0] = 0xc0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = mus_data[mus_data_ofs + 2];
                         mus_event[2] = 0;
                         mus_event[3] = 0;
                         break;
                     case 1:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 0;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 2:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 1;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 3:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 7;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 4:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 10;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 5:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 11;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 6:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 91;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 7:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 93;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 8:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 64;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     case 9:  
                         mus_event[0] = 0xb0 | (mus_data[mus_data_ofs] & 0x0f);
                         mus_event[1] = 67;
                         mus_event[2] = mus_data[mus_data_ofs + 2];
                         mus_event[3] = 0;
                         break;
                     default:  
                         goto _mus_next_data;
                 }
                 break;
             case 5:
                 mus_event_size = 1;
                 goto _mus_next_data;
                 break;
             case 6:
                 goto _mus_end_of_song;
                 break;
             case 7:
                 mus_event_size = 1;
                 goto _mus_next_data;
                  break;
          }
  
        setup_ret = _WM_SetupMidiEvent(mus_mdi, (uint8_t *)mus_event, 0);
//         setup_ret = _WM_SetupMidiEvent(mus_mdi, (uint8_t *)mus_event, MUS_SZ, 0);
          if (setup_ret == 0) {
              goto _mus_end;
          }
 
     _mus_next_data:
         if (!(mus_data[mus_data_ofs] & 0x80)) {
             mus_data_ofs += mus_event_size;
             goto _mus_build_event;
 
         }
         mus_data_ofs += mus_event_size;
 
         mus_ticks = 0;
         do {
             mus_ticks = (mus_ticks << 7) | (mus_data[mus_data_ofs++] & 0x7f);
         } while (mus_data[mus_data_ofs - 1] & 0x80);
 
         sample_count_f = ((float)mus_ticks * samples_per_tick_f) + sample_remainder;
         sample_count = (uint32_t)sample_count_f;
         sample_remainder = sample_count_f - (float)sample_count;
 
         mus_mdi->events[mus_mdi->event_count - 1].samples_to_next = sample_count;
         mus_mdi->extra_info.approx_total_samples += sample_count;
 
     } while (mus_data_ofs < mus_size);
 
 _mus_end_of_song:
     if ((mus_mdi->reverb = _WM_init_reverb(_WM_SampleRate, _WM_reverb_room_width, _WM_reverb_room_length, _WM_reverb_listen_posx, _WM_reverb_listen_posy)) == NULL) {
         _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, "to init reverb", 0);
         goto _mus_end;
     }
     _WM_midi_setup_endoftrack(mus_mdi);
     mus_mdi->extra_info.current_sample = 0;
     mus_mdi->current_event = &mus_mdi->events[0];
     mus_mdi->samples_to_mix = 0;
     mus_mdi->note = NULL;
 
     _WM_ResetToStart(mus_mdi);
 
 _mus_end:
     free(mus_mid_instr);
     if (mus_mdi->reverb) return (mus_mdi);
     _WM_freeMDI(mus_mdi);
     return NULL;
 }
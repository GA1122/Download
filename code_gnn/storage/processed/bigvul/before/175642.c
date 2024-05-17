static CHANNEL_MODE getChannelMode(OMX_U32 nChannels) {
    CHANNEL_MODE chMode = MODE_INVALID;
 switch (nChannels) {
 case 1: chMode = MODE_1; break;
 case 2: chMode = MODE_2; break;
 case 3: chMode = MODE_1_2; break;
 case 4: chMode = MODE_1_2_1; break;
 case 5: chMode = MODE_1_2_2; break;
 case 6: chMode = MODE_1_2_2_1; break;
 default: chMode = MODE_INVALID;
 }
 return chMode;
}

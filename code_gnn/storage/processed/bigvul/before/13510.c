static const uint8_t* dv_extract_pack(uint8_t* frame, enum dv_pack_type t)
{
    int offs;

    switch (t) {
    case dv_audio_source:
        offs = (80*6 + 80*16*3 + 3);
        break;
    case dv_audio_control:
        offs = (80*6 + 80*16*4 + 3);
        break;
    case dv_video_control:
        offs = (80*5 + 48 + 5);
        break;
    default:
        return NULL;
    }

    return frame[offs] == t ? &frame[offs] : NULL;
}

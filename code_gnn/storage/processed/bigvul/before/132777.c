PepperVideoRenderer3D::FrameDecodeTimestamp::FrameDecodeTimestamp(
    uint32_t frame_id,
    base::TimeTicks decode_started_time)
    : frame_id(frame_id), decode_started_time(decode_started_time) {
}

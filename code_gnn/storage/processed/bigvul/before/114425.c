DXVAVideoDecodeAccelerator::DXVAPictureBuffer::DXVAPictureBuffer(
    const media::PictureBuffer& buffer)
    : available_(true),
      picture_buffer_(buffer),
      decoding_surface_(NULL) {
}

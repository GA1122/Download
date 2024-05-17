void ImageResource::UpdateImage(
    scoped_refptr<SharedBuffer> shared_buffer,
    ImageResourceContent::UpdateImageOption update_image_option,
    bool all_data_received) {
  bool is_multipart = !!multipart_parser_;
  auto result = GetContent()->UpdateImage(std::move(shared_buffer), GetStatus(),
                                          update_image_option,
                                          all_data_received, is_multipart);
  if (result == ImageResourceContent::UpdateImageResult::kShouldDecodeError) {
    DecodeError(all_data_received);
  }
}

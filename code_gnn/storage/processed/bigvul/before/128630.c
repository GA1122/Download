std::string ContextualSearchDelegate::GetDiscourseContext(
    const ContextualSearchContext& context) {
  discourse_context::ClientDiscourseContext proto;
  discourse_context::Display* display = proto.add_display();
  display->set_uri(context.page_url.spec());

  discourse_context::Media* media = display->mutable_media();
  media->set_mime_type(context.encoding);

  discourse_context::Selection* selection = display->mutable_selection();
  selection->set_content(UTF16ToUTF8(context.surrounding_text));
  selection->set_start(context.start_offset);
  selection->set_end(context.end_offset);
  selection->set_is_uri_encoded(false);

  std::string serialized;
  proto.SerializeToString(&serialized);

  std::string encoded_context;
  base::Base64Encode(serialized, &encoded_context);
  std::replace(encoded_context.begin(), encoded_context.end(), '+', '-');
  std::replace(encoded_context.begin(), encoded_context.end(), '/', '_');
  return kDiscourseContextHeaderPrefix + encoded_context;
}

static void FormatWebGLStatusString(const StringView& gl_info,
                                    const StringView& info_string,
                                    StringBuilder& builder) {
  if (info_string.IsEmpty())
    return;
  builder.Append(", ");
  builder.Append(gl_info);
  builder.Append(" = ");
  builder.Append(info_string);
}

void FormatConverter::Convert() {
  if (SrcFormat == DstFormat &&
      alphaOp == WebGLImageConversion::kAlphaDoNothing) {
    NOTREACHED();
    return;
  }
  if (!IsFloatFormat<DstFormat>::value && IsFloatFormat<SrcFormat>::value) {
    NOTREACHED();
    return;
  }

  const bool src_format_comes_from_dom_element_or_image_data =
      WebGLImageConversion::SrcFormatComeFromDOMElementOrImageData(SrcFormat);
  if (!src_format_comes_from_dom_element_or_image_data &&
      SrcFormat != DstFormat) {
    NOTREACHED();
    return;
  }
  if (!src_format_comes_from_dom_element_or_image_data &&
      alphaOp == WebGLImageConversion::kAlphaDoUnmultiply) {
    NOTREACHED();
    return;
  }
  if (src_format_comes_from_dom_element_or_image_data &&
      alphaOp == WebGLImageConversion::kAlphaDoUnmultiply &&
      !SupportsConversionFromDomElements<DstFormat>::value) {
    NOTREACHED();
    return;
  }
  if ((!HasAlpha(SrcFormat) || !HasColor(SrcFormat) || !HasColor(DstFormat)) &&
      alphaOp != WebGLImageConversion::kAlphaDoNothing) {
    NOTREACHED();
    return;
  }
  if (src_format_comes_from_dom_element_or_image_data &&
      SrcFormat != DstFormat &&
      (DstFormat == WebGLImageConversion::kDataFormatRGB5999 ||
       DstFormat == WebGLImageConversion::kDataFormatRGB10F11F11F)) {
    NOTREACHED();
    return;
  }

  typedef typename DataTypeForFormat<SrcFormat>::Type SrcType;
  typedef typename DataTypeForFormat<DstFormat>::Type DstType;
  const int kIntermFormat = IntermediateFormat<DstFormat>::value;
  typedef typename DataTypeForFormat<kIntermFormat>::Type IntermType;
  const ptrdiff_t src_stride_in_elements = src_stride_ / sizeof(SrcType);
  const ptrdiff_t dst_stride_in_elements = dst_stride_ / sizeof(DstType);
  const bool kTrivialUnpack = SrcFormat == kIntermFormat;
  const bool kTrivialPack = DstFormat == kIntermFormat &&
                            alphaOp == WebGLImageConversion::kAlphaDoNothing;
  DCHECK(!kTrivialUnpack || !kTrivialPack);

  const SrcType* src_row_start =
      static_cast<const SrcType*>(static_cast<const void*>(
          static_cast<const uint8_t*>(src_start_) +
          ((src_stride_ * src_sub_rectangle_.Y()) + src_row_offset_)));

  if (dst_stride_ < 0 && depth_ > 1) {
    src_row_start -=
        (depth_ - 1) * src_stride_in_elements * unpack_image_height_;
  }

  DstType* dst_row_start = static_cast<DstType*>(dst_start_);
  if (kTrivialUnpack) {
    for (int d = 0; d < depth_; ++d) {
      for (int i = 0; i < src_sub_rectangle_.Height(); ++i) {
        Pack<DstFormat, alphaOp>(src_row_start, dst_row_start,
                                 src_sub_rectangle_.Width());
        src_row_start += src_stride_in_elements;
        dst_row_start += dst_stride_in_elements;
      }
      src_row_start += src_stride_in_elements *
                       (unpack_image_height_ - src_sub_rectangle_.Height());
    }
  } else if (kTrivialPack) {
    for (int d = 0; d < depth_; ++d) {
      for (int i = 0; i < src_sub_rectangle_.Height(); ++i) {
        Unpack<SrcFormat>(src_row_start, dst_row_start,
                          src_sub_rectangle_.Width());
        src_row_start += src_stride_in_elements;
        dst_row_start += dst_stride_in_elements;
      }
      src_row_start += src_stride_in_elements *
                       (unpack_image_height_ - src_sub_rectangle_.Height());
    }
  } else {
    for (int d = 0; d < depth_; ++d) {
      for (int i = 0; i < src_sub_rectangle_.Height(); ++i) {
        Unpack<SrcFormat>(src_row_start,
                          reinterpret_cast<IntermType*>(
                              unpacked_intermediate_src_data_.get()),
                          src_sub_rectangle_.Width());
        Pack<DstFormat, alphaOp>(reinterpret_cast<IntermType*>(
                                     unpacked_intermediate_src_data_.get()),
                                 dst_row_start, src_sub_rectangle_.Width());
        src_row_start += src_stride_in_elements;
        dst_row_start += dst_stride_in_elements;
      }
      src_row_start += src_stride_in_elements *
                       (unpack_image_height_ - src_sub_rectangle_.Height());
    }
  }
  success_ = true;
  return;
}

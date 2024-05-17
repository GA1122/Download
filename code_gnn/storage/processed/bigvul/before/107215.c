AutoFillUploadXmlParser::AutoFillUploadXmlParser(double* positive_upload_rate,
                                                 double* negative_upload_rate)
    : succeeded_(false),
      positive_upload_rate_(positive_upload_rate),
      negative_upload_rate_(negative_upload_rate) {
  DCHECK(positive_upload_rate_);
  DCHECK(negative_upload_rate_);
}

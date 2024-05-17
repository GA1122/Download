int XfrmController::fillNlAttrXfrmOutputMark(const __u32 output_mark_value,
                                             nlattr_xfrm_output_mark* output_mark) {
 if (output_mark_value == 0) {
 return 0;
 }

    output_mark->outputMark = output_mark_value;
 int len = NLA_HDRLEN + sizeof(__u32);
    fillXfrmNlaHdr(&output_mark->hdr, XFRMA_OUTPUT_MARK, len);
 return len;
}

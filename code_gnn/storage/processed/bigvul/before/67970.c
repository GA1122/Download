static int jp2_jp_getdata(jp2_box_t *box, jas_stream_t *in)
{
	jp2_jp_t *jp = &box->data.jp;
	if (jp2_getuint32(in, &jp->magic)) {
		return -1;
	}
	return 0;
}
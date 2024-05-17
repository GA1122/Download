static void jp2_colr_destroy(jp2_box_t *box)
{
	jp2_colr_t *colr = &box->data.colr;
	if (colr->iccp) {
		jas_free(colr->iccp);
	}
}
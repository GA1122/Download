box_gt(PG_FUNCTION_ARGS)
{
	BOX		   *box1 = PG_GETARG_BOX_P(0);
	BOX		   *box2 = PG_GETARG_BOX_P(1);

	PG_RETURN_BOOL(FPgt(box_ar(box1), box_ar(box2)));
}
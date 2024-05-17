static int ok_to_move_to_unc(struct rle_context *rlectx)
{

	if(rlectx->unc_len==0) {
		return (rlectx->run_len<4);
	}
	else if(rlectx->unc_len<=2) {
		return (rlectx->run_len<6);
	}
	else {
		return (rlectx->run_len<8);
	}
	return 0;
}

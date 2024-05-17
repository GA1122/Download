BaseShadow::holdJobAndExit( const char* reason, int hold_reason_code, int hold_reason_subcode )
{
	holdJob(reason,hold_reason_code,hold_reason_subcode);

	DC_Exit( JOB_SHOULD_HOLD );
}

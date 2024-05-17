import_print_stats (void *hd)
{
    struct stats_s *stats = hd;

    if( !opt.quiet ) {
	log_info(_("Total number processed: %lu\n"), stats->count );
	if( stats->skipped_new_keys )
	    log_info(_("      skipped new keys: %lu\n"),
						stats->skipped_new_keys );
	if( stats->no_user_id )
	    log_info(_("          w/o user IDs: %lu\n"), stats->no_user_id );
	if( stats->imported || stats->imported_rsa ) {
	    log_info(_("              imported: %lu"), stats->imported );
	    if (stats->imported_rsa)
              log_printf ("  (RSA: %lu)", stats->imported_rsa );
	    log_printf ("\n");
	}
	if( stats->unchanged )
	    log_info(_("             unchanged: %lu\n"), stats->unchanged );
	if( stats->n_uids )
	    log_info(_("          new user IDs: %lu\n"), stats->n_uids );
	if( stats->n_subk )
	    log_info(_("           new subkeys: %lu\n"), stats->n_subk );
	if( stats->n_sigs )
	    log_info(_("        new signatures: %lu\n"), stats->n_sigs );
	if( stats->n_revoc )
	    log_info(_("   new key revocations: %lu\n"), stats->n_revoc );
	if( stats->secret_read )
	    log_info(_("      secret keys read: %lu\n"), stats->secret_read );
	if( stats->secret_imported )
	    log_info(_("  secret keys imported: %lu\n"), stats->secret_imported );
	if( stats->secret_dups )
	    log_info(_(" secret keys unchanged: %lu\n"), stats->secret_dups );
	if( stats->not_imported )
	    log_info(_("          not imported: %lu\n"), stats->not_imported );
	if( stats->n_sigs_cleaned)
	    log_info(_("    signatures cleaned: %lu\n"),stats->n_sigs_cleaned);
	if( stats->n_uids_cleaned)
	    log_info(_("      user IDs cleaned: %lu\n"),stats->n_uids_cleaned);
    }

    if( is_status_enabled() ) {
	char buf[14*20];
	sprintf(buf, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
		stats->count,
		stats->no_user_id,
		stats->imported,
		stats->imported_rsa,
		stats->unchanged,
		stats->n_uids,
		stats->n_subk,
		stats->n_sigs,
		stats->n_revoc,
		stats->secret_read,
		stats->secret_imported,
		stats->secret_dups,
		stats->skipped_new_keys,
                stats->not_imported );
	write_status_text( STATUS_IMPORT_RES, buf );
    }
 }

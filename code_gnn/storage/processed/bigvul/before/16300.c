addConstraint( const char *constraint )
{
	static bool has_clause = false;
	if( has_clause ) {
		global_constraint += " && (";
	} else {
		global_constraint += "(";
	}
	global_constraint += constraint;
	global_constraint += ")";

	has_clause = true;
	has_constraint = true;
}

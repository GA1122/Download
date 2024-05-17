enum act_parse_ret parse_http_set_status(const char **args, int *orig_arg, struct proxy *px,
                                         struct act_rule *rule, char **err)
{
	char *error;

	rule->action = ACT_CUSTOM;
	rule->action_ptr = action_http_set_status;

	 
	if (!*args[*orig_arg]) {
		memprintf(err, "expects 1 argument: <status>; or 3 arguments: <status> reason <fmt>");
		return ACT_RET_PRS_ERR;
	}

	 
	rule->arg.status.code = strtol(args[*orig_arg], &error, 10);
	if (*error != '\0' || rule->arg.status.code < 100 || rule->arg.status.code > 999) {
		memprintf(err, "expects an integer status code between 100 and 999");
		return ACT_RET_PRS_ERR;
	}

	(*orig_arg)++;

	 
	rule->arg.status.reason = NULL;  
	if (*args[*orig_arg] && strcmp(args[*orig_arg], "reason") == 0 &&
	    (*args[*orig_arg + 1] && strcmp(args[*orig_arg + 1], "if") != 0 && strcmp(args[*orig_arg + 1], "unless") != 0)) {
		(*orig_arg)++;
		rule->arg.status.reason = strdup(args[*orig_arg]);
		(*orig_arg)++;
	}

	return ACT_RET_PRS_OK;
}

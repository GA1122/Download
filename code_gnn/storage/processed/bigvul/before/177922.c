  void add_param_to_argv(char *parsestart, int line)
  {
	int quote_open = 0, escaped = 0, param_len = 0;
	char param_buffer[1024], *curchar;
// 	int quote_open = 0, escaped = 0;
// 	struct xt_param_buf param = {};
// 	char *curchar;
  
  	 
 			param_buffer[param_len++] = *curchar;
 
 			if (param_len >= sizeof(param_buffer))
 				xtables_error(PARAMETER_PROBLEM,
  		case ' ':
  		case '\t':
  		case '\n':
			if (!param_len) {
// 			if (!param.len) {
  				 
  				continue;
  			}
  			break;
  		default:
  			 
			param_buffer[param_len++] = *curchar;
			if (param_len >= sizeof(param_buffer))
				xtables_error(PARAMETER_PROBLEM,
					      "Parameter too long!");
// 			add_param(&param, curchar);
  			continue;
  		}
  
		param_buffer[param_len] = '\0';
// 		param.buffer[param.len] = '\0';
  
  		 
		if ((param_buffer[0] == '-' &&
		     param_buffer[1] != '-' &&
		     strchr(param_buffer, 't')) ||
		    (!strncmp(param_buffer, "--t", 3) &&
		     !strncmp(param_buffer, "--table", strlen(param_buffer)))) {
// 		if ((param.buffer[0] == '-' &&
// 		     param.buffer[1] != '-' &&
// 		     strchr(param.buffer, 't')) ||
// 		    (!strncmp(param.buffer, "--t", 3) &&
// 		     !strncmp(param.buffer, "--table", strlen(param.buffer)))) {
  			xtables_error(PARAMETER_PROBLEM,
  				      "The -t option (seen in line %u) cannot be used in %s.\n",
  				      line, xt_params->program_name);
  		}
  
		add_argv(param_buffer, 0);
		param_len = 0;
// 		add_argv(param.buffer, 0);
// 		param.len = 0;
  	}
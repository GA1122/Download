 static int apparmor_setprocattr(struct task_struct *task, char *name,
 				void *value, size_t size)
 {
 	char *command, *args = value;
 	size_t arg_size;
 	int error;
 
 	if (size == 0)
 		return -EINVAL;
 	 
 	if (args[size - 1] != '\0') {
 		if (size == PAGE_SIZE)
 			return -EINVAL;
 		args[size] = '\0';
 	}
 
 	 
 	if (current != task)
 		return -EACCES;
 
 	args = value;
 	args = strim(args);
 	command = strsep(&args, " ");
 	if (!args)
 		return -EINVAL;
 	args = skip_spaces(args);
 	if (!*args)
 		return -EINVAL;
 
 	arg_size = size - (args - (char *) value);
 	if (strcmp(name, "current") == 0) {
 		if (strcmp(command, "changehat") == 0) {
 			error = aa_setprocattr_changehat(args, arg_size,
 							 !AA_DO_TEST);
 		} else if (strcmp(command, "permhat") == 0) {
 			error = aa_setprocattr_changehat(args, arg_size,
 							 AA_DO_TEST);
 		} else if (strcmp(command, "changeprofile") == 0) {
 			error = aa_setprocattr_changeprofile(args, !AA_ONEXEC,
 							     !AA_DO_TEST);
 		} else if (strcmp(command, "permprofile") == 0) {
 			error = aa_setprocattr_changeprofile(args, !AA_ONEXEC,
 							     AA_DO_TEST);
 		} else if (strcmp(command, "permipc") == 0) {
 			error = aa_setprocattr_permipc(args);
 		} else {
 			struct common_audit_data sa;
 			COMMON_AUDIT_DATA_INIT(&sa, NONE);
  			sa.aad.op = OP_SETPROCATTR;
  			sa.aad.info = name;
  			sa.aad.error = -EINVAL;
			return aa_audit(AUDIT_APPARMOR_DENIED, NULL, GFP_KERNEL,
// 			return aa_audit(AUDIT_APPARMOR_DENIED,
// 					__aa_current_profile(), GFP_KERNEL,
  					&sa, NULL);
  		}
  	} else if (strcmp(name, "exec") == 0) {
 		error = aa_setprocattr_changeprofile(args, AA_ONEXEC,
 						     !AA_DO_TEST);
 	} else {
 		 
 		return -EINVAL;
 	}
 	if (!error)
 		error = size;
 	return error;
 }
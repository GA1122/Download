 static int apparmor_setprocattr(struct task_struct *task, char *name,
 				void *value, size_t size)
  {
  	struct common_audit_data sa;
  	struct apparmor_audit_data aad = {0,};
	char *command, *args = value;
// 	char *command, *largs = NULL, *args = value;
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
// 	 
// 	if (args[size - 1] != '\0') {
// 		 
// 		largs = args = kmalloc(size + 1, GFP_KERNEL);
// 		if (!args)
// 			return -ENOMEM;
// 		memcpy(args, value, size);
// 		args[size] = '\0';
// 	}
// 
// 	error = -EINVAL;
  	args = strim(args);
  	command = strsep(&args, " ");
  	if (!args)
		return -EINVAL;
// 		goto out;
  	args = skip_spaces(args);
  	if (!*args)
		return -EINVAL;
// 		goto out;
  
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
 		} else
 			goto fail;
 	} else if (strcmp(name, "exec") == 0) {
 		if (strcmp(command, "exec") == 0)
 			error = aa_setprocattr_changeprofile(args, AA_ONEXEC,
 							     !AA_DO_TEST);
 		else
  			goto fail;
  	} else
  		 
		return -EINVAL;
// 		goto fail;
  
  	if (!error)
  		error = size;
// out:
// 	kfree(largs);
  	return error;
  
  fail:
 	sa.type = LSM_AUDIT_DATA_NONE;
 	sa.aad = &aad;
  	aad.profile = aa_current_profile();
  	aad.op = OP_SETPROCATTR;
  	aad.info = name;
	aad.error = -EINVAL;
// 	aad.error = error = -EINVAL;
  	aa_audit_msg(AUDIT_APPARMOR_DENIED, &sa, NULL);
	return -EINVAL;
// 	goto out;
  }
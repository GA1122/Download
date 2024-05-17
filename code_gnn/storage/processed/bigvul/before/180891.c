 static int __fpu__restore_sig(void __user *buf, void __user *buf_fx, int size)
 {
 	int ia32_fxstate = (buf != buf_fx);
 	struct task_struct *tsk = current;
 	struct fpu *fpu = &tsk->thread.fpu;
 	int state_size = fpu_kernel_xstate_size;
 	u64 xfeatures = 0;
 	int fx_only = 0;
 
 	ia32_fxstate &= (IS_ENABLED(CONFIG_X86_32) ||
 			 IS_ENABLED(CONFIG_IA32_EMULATION));
 
 	if (!buf) {
 		fpu__clear(fpu);
 		return 0;
 	}
 
 	if (!access_ok(VERIFY_READ, buf, size))
 		return -EACCES;
 
 	fpu__activate_curr(fpu);
 
 	if (!static_cpu_has(X86_FEATURE_FPU))
 		return fpregs_soft_set(current, NULL,
 				       0, sizeof(struct user_i387_ia32_struct),
 				       NULL, buf) != 0;
 
 	if (use_xsave()) {
 		struct _fpx_sw_bytes fx_sw_user;
 		if (unlikely(check_for_xstate(buf_fx, buf_fx, &fx_sw_user))) {
 			 
 			state_size = sizeof(struct fxregs_state);
 			fx_only = 1;
 			trace_x86_fpu_xstate_check_failed(fpu);
 		} else {
 			state_size = fx_sw_user.xstate_size;
 			xfeatures = fx_sw_user.xfeatures;
 		}
 	}
 
 	if (ia32_fxstate) {
 		 
 		struct fpu *fpu = &tsk->thread.fpu;
 		struct user_i387_ia32_struct env;
 		int err = 0;
 
 		 
  		fpu__drop(fpu);
  
		if (using_compacted_format())
// 		if (using_compacted_format()) {
  			err = copy_user_to_xstate(&fpu->state.xsave, buf_fx);
		else
// 		} else {
  			err = __copy_from_user(&fpu->state.xsave, buf_fx, state_size);
  
// 			 
// 			if (!err && state_size > offsetof(struct xregs_state, header) && fpu->state.xsave.header.xcomp_bv)
// 				err = -EINVAL;
// 		}
// 
  		if (err || __copy_from_user(&env, buf, sizeof(env))) {
  			fpstate_init(&fpu->state);
  			trace_x86_fpu_init_state(fpu);
 			err = -1;
 		} else {
 			sanitize_restored_xstate(tsk, &env, xfeatures, fx_only);
 		}
 
 		fpu->fpstate_active = 1;
 		preempt_disable();
 		fpu__restore(fpu);
 		preempt_enable();
 
 		return err;
 	} else {
 		 
 		user_fpu_begin();
 		if (copy_user_to_fpregs_zeroing(buf_fx, xfeatures, fx_only)) {
 			fpu__clear(fpu);
 			return -1;
 		}
 	}
 
 	return 0;
 }
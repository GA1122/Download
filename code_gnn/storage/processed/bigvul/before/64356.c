static int cmd_thread(void *data, const char *input) {
	RCore *core = (RCore*) data;
	if (r_sandbox_enable (0)) {
		eprintf ("This command is disabled in sandbox mode\n");
		return 0;
	}
	switch (input[0]) {
	case '\0':
	case 'j':
		r_core_task_list (core, *input);
		break;
	case '&':
		if (input[1] == '&') {
		} else {
			int tid = r_num_math (core->num, input + 1);
			if (tid) {
				RCoreTask *task = r_core_task_get (core, tid);
				if (task) {
					r_core_task_join (core, task);
				} else eprintf ("Cannot find task\n");
			} else {
				r_core_task_run (core, NULL);
			}
		}
		break;
	case '=': {
		int tid = r_num_math (core->num, input + 1);
		if (tid) {
			RCoreTask *task = r_core_task_get (core, tid);
			if (task) {
				r_cons_printf ("Task %d Status %c Command %s\n",
					task->id, task->state, task->msg->text);
				if (task->msg->res)
					r_cons_println (task->msg->res);
			} else eprintf ("Cannot find task\n");
		} else {
			r_core_task_list (core, 1);
		}}
		break;
	case '+':
		r_core_task_add (core, r_core_task_new (core, input + 1, (RCoreTaskCallback)task_finished, core));
		break;
	case '-':
		if (input[1] == '*') {
			r_core_task_del (core, -1);
		} else {
			r_core_task_del (core, r_num_math (core->num, input + 1));
		}
		break;
	case '?':
		{
			helpCmdTasks (core);
		}
		break;
	case ' ':
		{
			int tid = r_num_math (core->num, input + 1);
			if (tid) {
				RCoreTask *task = r_core_task_get (core, tid);
				if (task) {
					r_core_task_join (core, task);
				} else {
					eprintf ("Cannot find task\n");
				}
			} else {
				RCoreTask *task = r_core_task_add (core, r_core_task_new (
							core, input + 1, (RCoreTaskCallback)task_finished, core));
				RThread *th = r_th_new (taskbgrun, task, 0);
				task->msg->th = th;
			}
		}
		break;
	default:
		eprintf ("&?\n");
		break;
	}
	return 0;
}
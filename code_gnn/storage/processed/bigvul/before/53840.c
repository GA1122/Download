acpi_status acpi_os_execute(acpi_execute_type type,
			    acpi_osd_exec_callback function, void *context)
{
	acpi_status status = AE_OK;
	struct acpi_os_dpc *dpc;
	struct workqueue_struct *queue;
	int ret;
	ACPI_DEBUG_PRINT((ACPI_DB_EXEC,
			  "Scheduling function [%p(%p)] for deferred execution.\n",
			  function, context));

	if (type == OSL_DEBUGGER_MAIN_THREAD) {
		ret = acpi_debugger_create_thread(function, context);
		if (ret) {
			pr_err("Call to kthread_create() failed.\n");
			status = AE_ERROR;
		}
		goto out_thread;
	}

	 

	dpc = kzalloc(sizeof(struct acpi_os_dpc), GFP_ATOMIC);
	if (!dpc)
		return AE_NO_MEMORY;

	dpc->function = function;
	dpc->context = context;

	 
	if (type == OSL_NOTIFY_HANDLER) {
		queue = kacpi_notify_wq;
		INIT_WORK(&dpc->work, acpi_os_execute_deferred);
	} else if (type == OSL_GPE_HANDLER) {
		queue = kacpid_wq;
		INIT_WORK(&dpc->work, acpi_os_execute_deferred);
	} else {
		pr_err("Unsupported os_execute type %d.\n", type);
		status = AE_ERROR;
	}

	if (ACPI_FAILURE(status))
		goto err_workqueue;

	 
	ret = queue_work_on(0, queue, &dpc->work);
	if (!ret) {
		printk(KERN_ERR PREFIX
			  "Call to queue_work() failed.\n");
		status = AE_ERROR;
	}
err_workqueue:
	if (ACPI_FAILURE(status))
		kfree(dpc);
out_thread:
	return status;
}

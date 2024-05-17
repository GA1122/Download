static int cqspi_wait_idle(struct cqspi_st *cqspi)
{
	const unsigned int poll_idle_retry = 3;
	unsigned int count = 0;
	unsigned long timeout;

	timeout = jiffies + msecs_to_jiffies(CQSPI_TIMEOUT_MS);
	while (1) {
		 
		if (cqspi_is_idle(cqspi))
			count++;
		else
			count = 0;

		if (count >= poll_idle_retry)
			return 0;

		if (time_after(jiffies, timeout)) {
			 
			dev_err(&cqspi->pdev->dev,
				"QSPI is still busy after %dms timeout.\n",
				CQSPI_TIMEOUT_MS);
			return -ETIMEDOUT;
		}

		cpu_relax();
	}
}
static void handle_drain_completion(struct ib_cq *cq,
				    struct mlx5_ib_drain_cqe *sdrain,
				    struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;

	if (cq->poll_ctx == IB_POLL_DIRECT) {
		while (wait_for_completion_timeout(&sdrain->done, HZ / 10) <= 0)
			ib_process_cq_direct(cq, -1);
		return;
	}

	if (mdev->state == MLX5_DEVICE_STATE_INTERNAL_ERROR) {
		struct mlx5_ib_cq *mcq = to_mcq(cq);
		bool triggered = false;
		unsigned long flags;

		spin_lock_irqsave(&dev->reset_flow_resource_lock, flags);
		 
		if (!mcq->mcq.reset_notify_added)
			mcq->mcq.reset_notify_added = 1;
		else
			triggered = true;
		spin_unlock_irqrestore(&dev->reset_flow_resource_lock, flags);

		if (triggered) {
			 
			switch (cq->poll_ctx) {
			case IB_POLL_SOFTIRQ:
				irq_poll_disable(&cq->iop);
				irq_poll_enable(&cq->iop);
				break;
			case IB_POLL_WORKQUEUE:
				cancel_work_sync(&cq->work);
				break;
			default:
				WARN_ON_ONCE(1);
			}
		}

		 
		mcq->mcq.comp(&mcq->mcq);
	}

	wait_for_completion(&sdrain->done);
}

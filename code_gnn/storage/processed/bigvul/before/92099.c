static void destroy_qp_user(struct mlx5_ib_dev *dev, struct ib_pd *pd,
			    struct mlx5_ib_qp *qp, struct mlx5_ib_qp_base *base)
{
	struct mlx5_ib_ucontext *context;

	context = to_mucontext(pd->uobject->context);
	mlx5_ib_db_unmap_user(context, &qp->db);
	if (base->ubuffer.umem)
		ib_umem_release(base->ubuffer.umem);

	 
	if (qp->bfregn != MLX5_IB_INVALID_BFREG)
		mlx5_ib_free_bfreg(dev, &context->bfregi, qp->bfregn);
}

static bool bd_may_claim(struct block_device *bdev, struct block_device *whole,
			 void *holder)
{
	if (bdev->bd_holder == holder)
		return true;	  
	else if (bdev->bd_holder != NULL)
		return false; 	  
	else if (bdev->bd_contains == bdev)
		return true;  	  

	else if (whole->bd_holder == bd_may_claim)
		return true; 	  
	else if (whole->bd_holder != NULL)
		return false;	  
	else
		return true;	  
}

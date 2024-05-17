 static int rd_build_device_space(struct rd_dev *rd_dev)
// static int rd_allocate_sgl_table(struct rd_dev *rd_dev, struct rd_dev_sg_table *sg_table,
// 				 u32 total_sg_needed, unsigned char init_payload)
  {
	u32 i = 0, j, page_offset = 0, sg_per_table, sg_tables, total_sg_needed;
// 	u32 i = 0, j, page_offset = 0, sg_per_table;
  	u32 max_sg_per_table = (RD_MAX_ALLOCATION_SIZE /
  				sizeof(struct scatterlist));
	struct rd_dev_sg_table *sg_table;
  	struct page *pg;
  	struct scatterlist *sg;
	if (rd_dev->rd_page_count <= 0) {
		pr_err("Illegal page count: %u for Ramdisk device\n",
			rd_dev->rd_page_count);
		return -EINVAL;
	}
	 
	if (rd_dev->rd_flags & RDF_NULLIO)
		return 0;
	total_sg_needed = rd_dev->rd_page_count;
	sg_tables = (total_sg_needed / max_sg_per_table) + 1;
	sg_table = kzalloc(sg_tables * sizeof(struct rd_dev_sg_table), GFP_KERNEL);
	if (!sg_table) {
		pr_err("Unable to allocate memory for Ramdisk"
			" scatterlist tables\n");
		return -ENOMEM;
	}
	rd_dev->sg_table_array = sg_table;
	rd_dev->sg_table_count = sg_tables;
// 	unsigned char *p;
  
  	while (total_sg_needed) {
  		sg_per_table = (total_sg_needed > max_sg_per_table) ?
 			max_sg_per_table : total_sg_needed;
 
 		sg = kzalloc(sg_per_table * sizeof(struct scatterlist),
 				GFP_KERNEL);
 		if (!sg) {
 			pr_err("Unable to allocate scatterlist array"
 				" for struct rd_dev\n");
 			return -ENOMEM;
 		}
 
 		sg_init_table(sg, sg_per_table);
 
 		sg_table[i].sg_table = sg;
 		sg_table[i].rd_sg_count = sg_per_table;
 		sg_table[i].page_start_offset = page_offset;
 		sg_table[i++].page_end_offset = (page_offset + sg_per_table)
 						- 1;
 
 		for (j = 0; j < sg_per_table; j++) {
 			pg = alloc_pages(GFP_KERNEL, 0);
 			if (!pg) {
 				pr_err("Unable to allocate scatterlist"
 					" pages for struct rd_dev_sg_table\n");
 				return -ENOMEM;
  			}
  			sg_assign_page(&sg[j], pg);
  			sg[j].length = PAGE_SIZE;
// 
// 			p = kmap(pg);
// 			memset(p, init_payload, PAGE_SIZE);
// 			kunmap(pg);
  		}
  
  		page_offset += sg_per_table;
  		total_sg_needed -= sg_per_table;
  	}
  
// 	return 0;
// }
// 
// static int rd_build_device_space(struct rd_dev *rd_dev)
// {
// 	struct rd_dev_sg_table *sg_table;
// 	u32 sg_tables, total_sg_needed;
// 	u32 max_sg_per_table = (RD_MAX_ALLOCATION_SIZE /
// 				sizeof(struct scatterlist));
// 	int rc;
// 
// 	if (rd_dev->rd_page_count <= 0) {
// 		pr_err("Illegal page count: %u for Ramdisk device\n",
// 		       rd_dev->rd_page_count);
// 		return -EINVAL;
// 	}
// 
// 	 
// 	if (rd_dev->rd_flags & RDF_NULLIO)
// 		return 0;
// 
// 	total_sg_needed = rd_dev->rd_page_count;
// 
// 	sg_tables = (total_sg_needed / max_sg_per_table) + 1;
// 
// 	sg_table = kzalloc(sg_tables * sizeof(struct rd_dev_sg_table), GFP_KERNEL);
// 	if (!sg_table) {
// 		pr_err("Unable to allocate memory for Ramdisk"
// 		       " scatterlist tables\n");
// 		return -ENOMEM;
// 	}
// 
// 	rd_dev->sg_table_array = sg_table;
// 	rd_dev->sg_table_count = sg_tables;
// 
// 	rc = rd_allocate_sgl_table(rd_dev, sg_table, total_sg_needed, 0x00);
// 	if (rc)
// 		return rc;
// 
  	pr_debug("CORE_RD[%u] - Built Ramdisk Device ID: %u space of"
		" %u pages in %u tables\n", rd_dev->rd_host->rd_host_id,
		rd_dev->rd_dev_id, rd_dev->rd_page_count,
		rd_dev->sg_table_count);
// 		 " %u pages in %u tables\n", rd_dev->rd_host->rd_host_id,
// 		 rd_dev->rd_dev_id, rd_dev->rd_page_count,
// 		 rd_dev->sg_table_count);
  
  	return 0;
  }
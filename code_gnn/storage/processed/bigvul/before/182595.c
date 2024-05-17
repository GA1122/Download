 static void i8042_stop(struct serio *serio)
  {
  	struct i8042_port *port = serio->port_data;
  
// 	spin_lock_irq(&i8042_lock);
  	port->exists = false;
// 	port->serio = NULL;
// 	spin_unlock_irq(&i8042_lock);
  
  	 
  	synchronize_irq(I8042_AUX_IRQ);
  	synchronize_irq(I8042_KBD_IRQ);
	port->serio = NULL;
  }
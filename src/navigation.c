/* 
 * Copyright (C) 2000 Rich Wareham <richwareham@users.sourceforge.net>
 * 
 * This file is part of libdvdnav, a DVD navigation library.
 * 
 * libdvdnav is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * libdvdnav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * $Id$
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dvdnav.h>
#include "dvdnav_internal.h"

#include "vm.h"

/* Navigation API calls */

dvdnav_status_t dvdnav_still_skip(dvdnav_t *this) {
  if(!this)
   return S_ERR;

  this->position_current.still = 0;
  this->skip_still = 1;

  return S_OK;
}

dvdnav_status_t dvdnav_get_number_of_titles(dvdnav_t *this, int *titles) {
  if(!this)
   return S_ERR;

  if(!titles) {
    printerr("Passed a NULL pointer");
    return S_ERR;
  }

  if(!this->started) {
    /* Start the VM */
    vm_start(this->vm);
    this->started = 1;
  }

  (*titles) = vm_get_vmgi(this->vm)->tt_srpt->nr_of_srpts;

  return S_OK;
}

dvdnav_status_t dvdnav_get_number_of_parts(dvdnav_t *this, int title, int *parts) {
  if(!this)
   return S_ERR;

  if(!parts) {
    printerr("Passed a NULL pointer");
    return S_ERR;
  }
  if(!this->started) {
    printerr("Virtual DVD machine not started.");
    return S_ERR;
  }
  if ((title < 1) || (title > vm_get_vmgi(this->vm)->tt_srpt->nr_of_srpts) ) {
    printerr("Passed a title number out of range");
    return S_ERR;
  }
  (*parts) = vm_get_vmgi(this->vm)->tt_srpt->title[title-1].nr_of_ptts;
  return S_OK;
}

dvdnav_status_t dvdnav_current_title_info(dvdnav_t *this, int *title, int *part) {
  if(!this || !this->vm)
   return S_ERR;

  if(!title || !part) {
    printerr("Passed a NULL pointer");
    return S_ERR;
  }

  return vm_get_current_title_part(this->vm, title, part);
}

dvdnav_status_t dvdnav_title_play(dvdnav_t *this, int title) {

  if(!this) {
    return S_ERR;
  }

  return dvdnav_part_play(this, title, 1);
}

dvdnav_status_t dvdnav_part_play(dvdnav_t *this, int title, int part) {

  if(!this) {
    return S_ERR;
  }

  return vm_jump_title_part(this->vm, title, part);
}

dvdnav_status_t dvdnav_part_play_auto_stop(dvdnav_t *this, int title,
					  int part, int parts_to_play) {
  /* Perform jump as per usual */

  return dvdnav_part_play(this, title, part);
  
  /* FIXME: Impement auto-stop */
  
  /* return S_OK;*/ 
}

dvdnav_status_t dvdnav_time_play(dvdnav_t *this, int title,
				unsigned long int time) {
  /* FIXME: Implement */
  
  return S_OK;
}

dvdnav_status_t dvdnav_stop(dvdnav_t *this) {
  if(!this)
   return S_ERR;

  /* Set the STOP flag */
  
  this->stop = 1;
  
  return S_OK;
}

dvdnav_status_t dvdnav_go_up(dvdnav_t *this) {
  if(!this)
   return S_ERR;

  /* A nice easy function... delegate to the VM */
  vm_go_up(this->vm);

  return S_OK;
}




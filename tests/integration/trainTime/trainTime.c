/**
 Trains Protocol: Middleware for Uniform and Totally Ordered Broadcasts
 Copyright: Copyright (C) 2010-2012
 Contact: michel.simatic@telecom-sudparis.eu

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 USA

 Developer(s): Michel Simatic, Arthur Foltz, Damien Graux, Nicolas Hascoet, Nathan Reboud
 */

#include <stdio.h>

#include "trainTime.h"

char *msgTypeToStr(MType mtype){
  static char s[64];
  switch (mtype) {
  case FIRST:
    return "FIRST";
  case FAKE_TRAIN:
    return "FAKE_TRAIN";
  case LAST:
    return "LAST";
  case WRITE_PHASE:
    return "WRITE_PHASE";
  case WRITE_V_PHASE:
    return "WRITE_V_PHASE";
  case STOP:
    return "STOP";

  default:
    sprintf(s, "Unknown (value = %d)", mtype);
    return s;
  }
}

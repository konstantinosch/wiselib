/***************************************************************************
 ** This file is part of the generic algorithm library Wiselib.           **
 ** Copyright (C) 2008,2009 by the Wisebed (www.wisebed.eu) project.      **
 **                                                                       **
 ** The Wiselib is free software: you can redistribute it and/or modify   **
 ** it under the terms of the GNU Lesser General Public License as        **
 ** published by the Free Software Foundation, either version 3 of the    **
 ** License, or (at your option) any later version.                       **
 **                                                                       **
 ** The Wiselib is distributed in the hope that it will be useful,        **
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of        **
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
 ** GNU Lesser General Public License for more details.                   **
 **                                                                       **
 ** You should have received a copy of the GNU Lesser General Public      **
 ** License along with the Wiselib.                                       **
 ** If not, see <http://www.gnu.org/licenses/>.                           **
 ***************************************************************************/
#ifndef __CONNECTOR_CONTIKI_OS_MODEL_H__
#define __CONNECTOR_CONTIKI_OS_MODEL_H__

#include "external_interface/default_return_values.h"
#include "external_interface/contiki/contiki_timer.h"
#include "external_interface/contiki/contiki_radio.h"
#include "external_interface/contiki/contiki_debug.h"
#include "external_interface/contiki/contiki_types.h"

#include "util/serialization/endian.h"

#include "external_interface/contiki/contiki_com_uart.h"
#include "external_interface/contiki/contiki_clock.h"
#include "external_interface/contiki/contiki_distance.h"
#include "external_interface/contiki/contiki_position.h"

namespace wiselib
{

   /** \brief Contiki implementation of \ref os_concept "Os Concept".
    *
    *  \ingroup os_concept
    *  \ingroup basic_return_values_concept
    *  \ingroup contiki_facets
    */
   class ContikiOsModel
      : public DefaultReturnValues<ContikiOsModel>
   {
   public:
      typedef ContikiOsModel AppMainParameter;

      typedef unsigned int size_t;
      typedef uint8_t block_data_t;

      typedef ContikiTimer<ContikiOsModel> Timer;
      typedef ContikiRadio<ContikiOsModel> Radio;
      typedef ContikiDebug<ContikiOsModel> Debug;

      typedef ContikiClockModel<ContikiOsModel> Clock;

      static const Endianness endianness = WISELIB_ENDIANNESS;
   };

}

#endif

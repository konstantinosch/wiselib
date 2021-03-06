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
#ifndef CONNECTOR_SHAWN_TXTSRADIOMODEL_H
#define CONNECTOR_SHAWN_TXTSRADIOMODEL_H

#include "external_interface/shawn/shawn_types.h"
#include <cmath>

namespace wiselib
{
   /** \brief OSA Implementation of \ref radio_concept "Radio concept"
   *  \ingroup radio_concept
   *
   * OSA implementation of the \ref radio_concept "Radio concept" ...
   */
   template<typename OsModel_P>
   class ShawnTxTsRadioModel
   {
   public:
      typedef OsModel_P OsModel;

      typedef ShawnTxTsRadioModel<OsModel> self_type;
      typedef self_type* self_pointer_t;

      typedef ExtIfaceProcessor::node_id_t node_id_t;
      typedef ExtIfaceProcessor::block_data_t block_data_t;
      typedef ExtIfaceProcessor::size_t size_t;
      typedef ExtIfaceProcessor::message_id_t message_id_t;
      typedef ExtIfaceProcessor::ExtendedData ExtendedData;

      typedef delegate3<void, int, long, unsigned char*> radio_delegate_t;
      // --------------------------------------------------------------------
      enum ErrorCodes
      {
         SUCCESS = OsModel::SUCCESS,
         ERR_UNSPEC = OsModel::ERR_UNSPEC,
         ERR_NOTIMPL = OsModel::ERR_NOTIMPL
      };
      // --------------------------------------------------------------------
      enum SpecialNodeIds {
         BROADCAST_ADDRESS = 0xffff, ///< All nodes in communication rnage
         NULL_NODE_ID      = -1      ///< Unknown/No node id
      };
      // --------------------------------------------------------------------
      enum Restrictions {
         MAX_MESSAGE_LENGTH = 0xff   ///< Maximal number of bytes in payload
      };
      // --------------------------------------------------------------------
      class TxPower;
      // --------------------------------------------------------------------
      ShawnTxTsRadioModel( ShawnOs& os )
         : os_(os)
      {}
      // --------------------------------------------------------------------
      int send( node_id_t id, size_t len, block_data_t *data )
      {
	      for( shawn::World::const_node_iterator
		       it = os().proc->owner().world().simulation_controller().world().begin_nodes();
		       it != os().proc->owner().world().simulation_controller().world().end_nodes();
		       ++it )
	      {
//	      (int)strtol(
//           os().proc->owner().label().substr((os().proc->owner().label()).rfind(":")+3, os().proc->owner().label().size()).data(), NULL, 16);
           
      int idx = (int)strtol(
           it->label().substr((it->label()).rfind(":")+3, it->label().size()).data(), NULL, 16);
	      
	      	if (idx == id || id == BROADCAST_ADDRESS) {
		 os().proc->send_wiselib_message( id, len, data );
		 return SUCCESS;
	      	}

	      }
		 return SUCCESS;

        
      //os().proc->send_wiselib_message( id, len, data );
//         return SUCCESS;
      };
      // --------------------------------------------------------------------
      int enable_radio()
      { return SUCCESS; }
      // --------------------------------------------------------------------
      int disable_radio()
      { return SUCCESS; }
      // --------------------------------------------------------------------
      node_id_t id()
      {
       return (int)strtol(
           os().proc->owner().label().substr((os().proc->owner().label()).rfind(":")+3, os().proc->owner().label().size()).data(), NULL, 16);
      
//         return os().proc->id();
      }
      // --------------------------------------------------------------------
      //---------- From concept VariablePowerRadio ------------
      int set_power(TxPower p);
      // --------------------------------------------------------------------
      TxPower power();
      //-------------------------------------------------------
      template<class T, void (T::*TMethod)(node_id_t, size_t, block_data_t*)>
      int reg_recv_callback( T *obj_pnt )
      {
         return os().proc->template reg_recv_callback<T, TMethod>( obj_pnt );
      }
      //-------------------------------------------------------
      template<class T, void (T::*TMethod)(node_id_t, size_t, block_data_t*, ExtendedData const &)>
      int reg_recv_callback( T *obj_pnt )
      {
          return os().proc->template reg_recv_callback<T, TMethod>( obj_pnt );
      }
      // --------------------------------------------------------------------
      int unreg_recv_callback( int idx )
      { return ERR_NOTIMPL; }
      // --------------------------------------------------------------------
      size_t reserved_bytes()
      {
      	return sizeof(message_id_t) + sizeof(size_t) + sizeof(uint16_t);
      };
      // --------------------------------------------------------------------
   private:
      ShawnOs& os()
      { return os_; }
      // --------------------------------------------------------------------
      ShawnOs& os_;
   };
   // --------------------------------------------------------------------
   /** \brief Shawn Implementation of \ref txpower_concept "TxPower Concept"
   *  \ingroup txpower_concept
   *
   *  Shawn implementation of the \ref txpower_concept "TxPower Concept" ...
   */
   template<typename OsModel_P>
   class ShawnTxTsRadioModel<OsModel_P>::TxPower {
   public:
      TxPower();

      bool operator==(TxPower) const;
      bool operator!=(TxPower) const;
      bool operator<=(TxPower) const;
      bool operator>=(TxPower) const;
      bool operator<(TxPower) const;
      bool operator>(TxPower) const;
      TxPower operator++();
      TxPower operator++(int);
      TxPower operator--();
      TxPower operator--(int);

      static TxPower from_ratio(int);
      void set_ratio(int);
      int to_ratio() const;
      static TxPower from_dB(int);
      void set_dB(int);
      int to_dB() const;

      void set_steps(int);
      int steps() const;
      static void set_default_steps(int);
      static int default_steps();

      static TxPower const MIN;
      static TxPower const MAX;

   private:
      TxPower(double);

      double value;
      int steps_;
      static int def_steps;

      friend class ShawnTxTsRadioModel<OsModel_P>;
   };
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower const ShawnTxTsRadioModel<OsModel_P>::TxPower::MIN=0.0;
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower const ShawnTxTsRadioModel<OsModel_P>::TxPower::MAX=1.0;
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   int ShawnTxTsRadioModel<OsModel_P>::TxPower::def_steps=10;
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline ShawnTxTsRadioModel<OsModel_P>::TxPower::TxPower(double v):
      value(v),
      steps_(def_steps){}
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline ShawnTxTsRadioModel<OsModel_P>::TxPower::TxPower():
      value(1.0),
      steps_(def_steps){}
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator==(TxPower p) const {
      return value==p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator!=(TxPower p) const {
      return value!=p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator<=(TxPower p) const {
      return value<=p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator>=(TxPower p) const {
      return value>=p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator<(TxPower p) const {
      return value<p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline bool ShawnTxTsRadioModel<OsModel_P>::TxPower::operator>(TxPower p) const {
      return value>p.value;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::operator++(){
      value=static_cast<int>(value*steps_+0.5)+1.0/steps_;
      return *this;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::operator++(int){
      TxPower p=*this;
      value=static_cast<int>(value*steps_+0.5)+1.0/steps_;
      return p;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::operator--(){
      value=static_cast<int>(value*steps_+0.5)-1.0/steps_;
      return *this;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::operator--(int){
      TxPower p=*this;
      value=static_cast<int>(value*steps_+0.5)-1.0/steps_;
      return p;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::from_ratio(int ratio){
      return TxPower(ratio/1e3);
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   void ShawnTxTsRadioModel<OsModel_P>::TxPower::set_ratio(int ratio){
      if(ratio>=0&&ratio<=1000)
         value=ratio/1e3;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   int ShawnTxTsRadioModel<OsModel_P>::TxPower::to_ratio() const {
      return value*1e3+.5;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::TxPower::from_dB(int db){
      return TxPower(std::pow(10.0,db/10.0));
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   void ShawnTxTsRadioModel<OsModel_P>::TxPower::set_dB(int db){
      if(db<=0)
         value=std::pow(10.0,db/10.0);
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   inline int ShawnTxTsRadioModel<OsModel_P>::TxPower::to_dB() const {
      return std::log10(value)*10.0+.5;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   void ShawnTxTsRadioModel<OsModel_P>::TxPower::set_steps(int n){
      if(n>0)
         steps_=n;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   int ShawnTxTsRadioModel<OsModel_P>::TxPower::steps() const {
      return steps_;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   void ShawnTxTsRadioModel<OsModel_P>::TxPower::set_default_steps(int n){
      if(n>0)
         def_steps=n;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   int ShawnTxTsRadioModel<OsModel_P>::TxPower::default_steps() {
      return def_steps;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   int ShawnTxTsRadioModel<OsModel_P>::set_power(TxPower p)
   {
     os().proc->owner_w().set_transmission_range(p.value);
     return SUCCESS;
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   typename ShawnTxTsRadioModel<OsModel_P>::TxPower ShawnTxTsRadioModel<OsModel_P>::power() {
     return TxPower(os().proc->owner_w().transmission_range());
   }
   //------------------------------------------------------------------------
   template<typename OsModel_P>
   void ShawnTxTsRadioModel<OsModel_P>::set_channel(int c)
   {}
   //------------------------------------------------------------------------

}

#endif

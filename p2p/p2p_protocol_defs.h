// Copyright (c) 2018, Ryo Currency Project
// Portions copyright (c) 2014-2018, The Monero Project
//
// Portions of this file are available under BSD-3 license. Please see ORIGINAL-LICENSE for details
// All rights reserved.
//
// Authors and copyright holders give permission for following:
//
// 1. Redistribution and use in source and binary forms WITHOUT modification.
//
// 2. Modification of the source form for your own personal use.
//
// As long as the following conditions are met:
//
// 3. You must not distribute modified copies of the work to third parties. This includes
//    posting the work online, or hosting copies of the modified work for download.
//
// 4. Any derivative version of this work is also covered by this license, including point 8.
//
// 5. Neither the name of the copyright holders nor the names of the authors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// 6. You agree that this licence is governed by and shall be construed in accordance
//    with the laws of England and Wales.
//
// 7. You agree to submit all disputes arising out of or in connection with this licence
//    to the exclusive jurisdiction of the Courts of England and Wales.
//
// Authors and copyright holders agree that:
//
// 8. This licence expires and the work covered by it is released into the
//    public domain on 1st of February 2019
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

//#include "cryptonote_config.h"
//#include "misc_language.h"
#include "../epee/net/net_utils_base.h"
#include "../epee/serialization/keyvalue_serialization.h"
//#include "string_tools.h"
//#include "time_helper.h"
#include <boost/uuid/uuid.hpp>
//#ifdef ALLOW_DEBUG_COMMANDS
//#include "crypto/crypto.h"
//#endif

namespace nodetool
{
typedef boost::uuids::uuid uuid;
typedef uint64_t peerid_type;

#pragma pack(push, 1)

struct network_address_old
{
	uint32_t ip;
	uint32_t port;

	BEGIN_KV_SERIALIZE_MAP()
	KV_SERIALIZE(ip)
	KV_SERIALIZE(port)
	END_KV_SERIALIZE_MAP()
};

template <typename AddressType>
struct peerlist_entry_base
{
	AddressType adr;
	peerid_type id;
	int64_t last_seen;

	BEGIN_KV_SERIALIZE_MAP()
	KV_SERIALIZE(adr)
	KV_SERIALIZE(id)
	KV_SERIALIZE(last_seen)
	END_KV_SERIALIZE_MAP()
};
typedef peerlist_entry_base<epee::net_utils::network_address> peerlist_entry;

#pragma pack(pop)

struct basic_node_data
{
	uuid network_id;
	uint64_t local_time;
	uint32_t my_port;
	peerid_type peer_id;

	BEGIN_KV_SERIALIZE_MAP()
	KV_SERIALIZE_VAL_POD_AS_BLOB(network_id)
	KV_SERIALIZE(peer_id)
	KV_SERIALIZE(local_time)
	KV_SERIALIZE(my_port)
	END_KV_SERIALIZE_MAP()
};

#define P2P_COMMANDS_POOL_BASE 1000

/************************************************************************/
/*                                                                      */
/************************************************************************/
template <class t_playload_type>
struct COMMAND_HANDSHAKE_T
{
	const static int ID = P2P_COMMANDS_POOL_BASE + 1;

	struct request
	{
		basic_node_data node_data;
		t_playload_type payload_data;

		BEGIN_KV_SERIALIZE_MAP()
		KV_SERIALIZE(node_data)
		KV_SERIALIZE(payload_data)
		END_KV_SERIALIZE_MAP()
	};

	struct response
	{
		basic_node_data node_data;
		t_playload_type payload_data;
		std::list<peerlist_entry> local_peerlist_new;
		
		BEGIN_KV_SERIALIZE_MAP()
		KV_SERIALIZE(node_data)
		KV_SERIALIZE(payload_data)
		if(is_store)
		{
			// saving: save both, so old and new peers can understand it
			KV_SERIALIZE(local_peerlist_new)
			std::list<peerlist_entry_base<network_address_old>> local_peerlist;
			for(const auto &p : this_ref.local_peerlist_new)
			{
				if(p.adr.get_type_id() == epee::net_utils::ipv4_network_address::ID)
				{
					const epee::net_utils::network_address &na = p.adr;
					const epee::net_utils::ipv4_network_address &ipv4 = na.as<const epee::net_utils::ipv4_network_address>();
					local_peerlist.push_back(peerlist_entry_base<network_address_old>({{ipv4.ip(), ipv4.port()}, p.id, p.last_seen}));
				}
				else
					MDEBUG("Not including in legacy peer list: " << p.adr.str());
			}
			epee::serialization::selector<is_store>::serialize_stl_container_pod_val_as_blob(local_peerlist, stg, hparent_section, "local_peerlist");
		}
		else
		{
			// loading: load old list only if there is no new one
			if(!epee::serialization::selector<is_store>::serialize(this_ref.local_peerlist_new, stg, hparent_section, "local_peerlist_new"))
			{
				std::list<peerlist_entry_base<network_address_old>> local_peerlist;
				epee::serialization::selector<is_store>::serialize_stl_container_pod_val_as_blob(local_peerlist, stg, hparent_section, "local_peerlist");
				for(const auto &p : local_peerlist)
					((response &)this_ref).local_peerlist_new.push_back(peerlist_entry({epee::net_utils::ipv4_network_address(p.adr.ip, p.adr.port), p.id, p.last_seen}));
			}
		}
		END_KV_SERIALIZE_MAP()
	};
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
}
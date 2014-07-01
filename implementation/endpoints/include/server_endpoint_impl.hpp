// Copyright (C) 2014 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef VSOMEIP_SERVER_IMPL_HPP
#define VSOMEIP_SERVER_IMPL_HPP

#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>

#include "endpoint_impl.hpp"

namespace vsomeip {

template < typename Protocol, int MaxBufferSize >
class server_endpoint_impl
		: public endpoint_impl< MaxBufferSize >,
		  public std::enable_shared_from_this< server_endpoint_impl< Protocol, MaxBufferSize > > {
public:
	typedef typename Protocol::socket socket_type;
	typedef typename Protocol::endpoint endpoint_type;
	typedef boost::array< uint8_t, MaxBufferSize > buffer_type;

	server_endpoint_impl(std::shared_ptr< endpoint_host > _host, endpoint_type _local, boost::asio::io_service &_io);

	bool is_client() const;

	bool send(const uint8_t *_data, uint32_t _size, bool _flush);
	bool flush();

public:
	void connect_cbk(boost::system::error_code const &_error);
	void send_cbk(boost::system::error_code const &_error, std::size_t _bytes);
	void flush_cbk(endpoint_type _target, const boost::system::error_code &_error);

public:
	virtual void send_queued() = 0;
	virtual endpoint_type get_remote() const = 0;

protected:
	typedef std::map< endpoint_type, std::deque< std::vector< byte_t > > > queue_type_t;
	queue_type_t packet_queues_;
	typename queue_type_t::iterator current_queue_;
	std::map< endpoint_type, std::vector< byte_t > > packetizer_;

	std::map< client_t, endpoint_type > clients_;

	boost::asio::system_timer flush_timer_;

	endpoint_type local_;

	std::mutex mutex_;

private:
	bool set_next_queue();
};

} // namespace vsomeip

#endif // VSOMEIP_SERVICE_ENDPOINT_IMPL_HPP

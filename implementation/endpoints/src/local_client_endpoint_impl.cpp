// Copyright (C) 2014 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iomanip>

#include <boost/asio/write.hpp>

#include <vsomeip/defines.hpp>

#include "../include/endpoint_host.hpp"
#include "../include/local_client_endpoint_impl.hpp"

namespace vsomeip {

local_client_endpoint_impl::local_client_endpoint_impl(
		std::shared_ptr< endpoint_host > _host, endpoint_type _remote, boost::asio::io_service &_io)
	: local_client_endpoint_base_impl(_host, _remote, _io) {
	is_supporting_magic_cookies_ = false;
}

void local_client_endpoint_impl::start() {
	connect();
}

void local_client_endpoint_impl::connect() {
	socket_.open(remote_.protocol());

	socket_.async_connect(
		remote_,
		std::bind(
			&local_client_endpoint_base_impl::connect_cbk,
			shared_from_this(),
			std::placeholders::_1
		)
	);
}

void local_client_endpoint_impl::receive() {
	socket_.async_receive(
		boost::asio::buffer(buffer_),
		std::bind(
			&local_client_endpoint_impl::receive_cbk,
			std::dynamic_pointer_cast< local_client_endpoint_impl >(shared_from_this()),
			std::placeholders::_1,
			std::placeholders::_2
		)
	);
}

void local_client_endpoint_impl::send_queued() {
	static byte_t its_start_tag[] = { 0x67, 0x37, 0x6D, 0x07 };
	static byte_t its_end_tag[] = { 0x07, 0x6D, 0x37, 0x67 };

	boost::asio::async_write(
		socket_,
		boost::asio::buffer(
			its_start_tag,
			sizeof(its_start_tag)
		),
		std::bind(
			&local_client_endpoint_impl::send_tag_cbk,
			std::dynamic_pointer_cast< local_client_endpoint_impl >(shared_from_this()),
			std::placeholders::_1,
			std::placeholders::_2
		)
	);

#if 0
	std::cout << "lce(s): ";
	for (std::size_t i = 0; i < packet_queue_.front().size(); i++)
		std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)packet_queue_.front()[i] << " ";
	std::cout << std::endl;
#endif

	boost::asio::async_write(
		socket_,
		boost::asio::buffer(
			&packet_queue_.front()[0],
			packet_queue_.front().size()
		),
		std::bind(
			&client_endpoint_impl::send_cbk,
			this->shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2
		)
	);

	boost::asio::async_write(
		socket_,
		boost::asio::buffer(
			its_end_tag,
			sizeof(its_end_tag)
		),
		std::bind(
			&local_client_endpoint_impl::send_tag_cbk,
			std::dynamic_pointer_cast< local_client_endpoint_impl >(shared_from_this()),
			std::placeholders::_1,
			std::placeholders::_2
		)
	);
}

void local_client_endpoint_impl::send_magic_cookie() {
}

void local_client_endpoint_impl::join(const std::string &) {
}

void local_client_endpoint_impl::leave(const std::string &) {
}

void local_client_endpoint_impl::send_tag_cbk(
		boost::system::error_code const &_error, std::size_t _bytes) {
}

void local_client_endpoint_impl::receive_cbk(
		boost::system::error_code const &_error, std::size_t _bytes) {
}

} // namespace vsomeip

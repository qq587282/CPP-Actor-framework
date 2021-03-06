#include "actor_socket.h"

tcp_socket::tcp_socket(io_engine& ios)
:_socket(ios), _nonBlocking(false)
#ifdef ENABLE_ASIO_PRE_OP
, _preOption(false)
#endif
{
#ifdef HAS_ASIO_SEND_FILE
#ifdef __linux__
	_sendFileState.offset = NULL;
	_sendFileState.count = 0;
	_sendFileState.fd = 0;
#endif
#endif
}

tcp_socket::~tcp_socket()
{}

tcp_socket::result tcp_socket::close()
{
	boost::system::error_code ec;
	_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	_socket.close(ec);
	return result{ 0, ec.value(), !ec };
}

tcp_socket::result tcp_socket::no_delay()
{
	boost::system::error_code ec;
	_socket.set_option(boost::asio::ip::tcp::no_delay(true), ec);
	return result{ 0, ec.value(), !ec };
}

void tcp_socket::pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	_preOption = true;
#endif
}

bool tcp_socket::is_pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	return _preOption;
#else
	return false;
#endif
}

bool tcp_socket::try_again(const result& res)
{
	return boost::asio::error::try_again == res.code || boost::asio::error::would_block == res.code;
}

boost::asio::ip::tcp::endpoint tcp_socket::make_endpoint(const char* remoteIp, unsigned short remotePort)
{
	return boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(remoteIp), remotePort);
}

std::string tcp_socket::local_endpoint(unsigned short& port)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint ep = _socket.local_endpoint(ec);
	if (!ec)
	{
		port = ep.port();
		return ep.address().to_string(ec);
	}
	port = 0;
	return std::string();
}

std::string tcp_socket::remote_endpoint(unsigned short& port)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint ep = _socket.remote_endpoint(ec);
	if (!ec)
	{
		port = ep.port();
		return ep.address().to_string(ec);
	}
	port = 0;
	return std::string();
}

void tcp_socket::set_internal_non_blocking()
{
	boost::system::error_code ec;
	using namespace boost::asio::detail;
	socket_ops::state_type state = socket_ops::user_set_non_blocking;
	_nonBlocking = socket_ops::set_internal_non_blocking(_socket.native_handle(), state, true, ec);
}

tcp_socket::result tcp_socket::connect(my_actor* host, const boost::asio::ip::tcp::endpoint& remoteEndpoint)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_connect(remoteEndpoint, std::move(h));
	});
}

tcp_socket::result tcp_socket::connect(my_actor* host, const char* remoteIp, unsigned short remotePort)
{
	return connect(host, make_endpoint(remoteIp, remotePort));
}

tcp_socket::result tcp_socket::read(my_actor* host, void* buff, size_t length)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_read(buff, length, std::move(h));
	});
}

tcp_socket::result tcp_socket::read_some(my_actor* host, void* buff, size_t length)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_read_some(buff, length, std::move(h));
	});
}

tcp_socket::result tcp_socket::write(my_actor* host, const void* buff, size_t length)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_write(buff, length, std::move(h));
	});
}

tcp_socket::result tcp_socket::write_some(my_actor* host, const void* buff, size_t length)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_write_some(buff, length, std::move(h));
	});
}

tcp_socket::result tcp_socket::timed_connect(my_actor* host, int ms, const boost::asio::ip::tcp::endpoint& remoteEndpoint)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_connect(remoteEndpoint, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_socket::timed_connect(my_actor* host, int ms, const char* remoteIp, unsigned short remotePort)
{
	return timed_connect(host, ms, make_endpoint(remoteIp, remotePort));
}

tcp_socket::result tcp_socket::timed_read(my_actor* host, int ms, void* buff, size_t length)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_read(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_socket::timed_read_some(my_actor* host, int ms, void* buff, size_t length)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_read_some(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_socket::timed_write(my_actor* host, int ms, const void* buff, size_t length)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_write(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_socket::timed_write_some(my_actor* host, int ms, const void* buff, size_t length)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_write_some(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_socket::try_write_same(const void* buff, size_t length)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::send(_socket.native_handle(), &buf, 1, 0, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

tcp_socket::result tcp_socket::try_read_same(void* buff, size_t length)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::recv(_socket.native_handle(), &buf, 1, 0, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

tcp_socket::result tcp_socket::try_mwrite_same(const void* const* buffs, const size_t* lengths, size_t count, size_t* lastBytes)
{
#ifdef ENABLE_SCK_MULTI_IO
	if (!lastBytes)
	{
		return _try_mwrite_same(buffs, lengths, count);
	}
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::sendmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, MSG_NOSIGNAL);
		if (pcks > 0)
		{
			res.s += pcks;
			const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
			*lastBytes = mhdr[pcks-1].msg_len;
			if (*lastBytes != lengths[lastTotPcks-1])
			{
				break;
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_write_same(buffs[i], lengths[i]);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		if (lastBytes)
		{
			res.s++;
			*lastBytes = tr.s;
		}
		else
		{
			res.s += tr.s;
		}
		if (tr.s != lengths[i])
		{
			break;
		}
	}
	res.ok = true;
	return res;
#endif
}

tcp_socket::result tcp_socket::try_mread_same(void* const* buffs, const size_t* lengths, size_t count, size_t* lastBytes)
{
#ifdef ENABLE_SCK_MULTI_IO
	if (!lastBytes)
	{
		return _try_mread_same(buffs, lengths, count);
	}
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::recvmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, 0, NULL);
		if (pcks > 0)
		{
			res.s += pcks;
			const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
			*lastBytes = mhdr[pcks-1].msg_len;
			if (*lastBytes != lengths[lastTotPcks-1])
			{
				break;
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_read_same(buffs[i], lengths[i]);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		if (lastBytes)
		{
			res.s++;
			*lastBytes = tr.s;
		}
		else
		{
			res.s += tr.s;
		}
		if (tr.s != lengths[i])
		{
			break;
		}
	}
	res.ok = true;
	return res;
#endif
}

tcp_socket::result tcp_socket::_try_mwrite_same(const void* const* buffs, const size_t* lengths, size_t count)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		size_t ct = 0;
		size_t currTotBytes = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			currTotBytes += lengths[k];
		}
		if (!ct)
		{
			break;
		}
		struct mmsghdr mhdr;
		memset(&mhdr, 0, sizeof(mhdr));
		mhdr.msg_hdr.msg_iov = msgs;
		mhdr.msg_hdr.msg_iovlen = ct;
		const int pcks = ::sendmmsg(_socket.native_handle(), &mhdr, 1, MSG_NOSIGNAL);
		if (pcks > 0)
		{
			res.s += mhdr.msg_len;
			if (mhdr.msg_len != currTotBytes)
			{
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	return result{ 0, 0, false };
#endif
}

tcp_socket::result tcp_socket::_try_mread_same(void* const* buffs, const size_t* lengths, size_t count)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		size_t ct = 0;
		size_t currTotBytes = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			currTotBytes += lengths[k];
		}
		if (!ct)
		{
			break;
		}
		struct mmsghdr mhdr;
		memset(&mhdr, 0, sizeof(mhdr));
		mhdr.msg_hdr.msg_iov = msgs;
		mhdr.msg_hdr.msg_iovlen = ct;
		const int pcks = ::recvmmsg(_socket.native_handle(), &mhdr, 1, 0, NULL);
		if (pcks > 0)
		{
			res.s += mhdr.msg_len;
			if (mhdr.msg_len != currTotBytes)
			{
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	return result{ 0, 0, false };
#endif
}

#ifdef HAS_ASIO_SEND_FILE
#ifdef __linux__
tcp_socket::result tcp_socket::try_send_file_same(int fd, unsigned long long* offset, size_t& length)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		if (0 == length)
		{
			if (!offset)
			{
				off_t currpos = ::lseek(fd, 0, SEEK_CUR);
				unsigned long long sl = (unsigned long long)::lseek(fd, 0, SEEK_END) - (unsigned long long)currpos;
				if (sl >> 32)
				{
					return res;
				}
				length = (size_t)sl;
				::lseek(fd, currpos, SEEK_SET);
			}
			else
			{
				unsigned long long sl = (unsigned long long)::lseek(fd, 0, SEEK_END) - *offset;
				if (sl >> 32)
				{
					return res;
				}
				length = (size_t)sl;
			}
			if (0 == length)
			{
				res.ok = true;
				return res;
			}
		}
		socket_ops::send_file_pck pck = { offset, length, fd };
		socket_ops::buf buf;
		socket_ops::init_buf(buf, &pck, -1);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::send(_socket.native_handle(), &buf, 1, 0, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}
#elif WIN32
bool tcp_socket::init_send_file(HANDLE hFile, unsigned long long* offset, size_t& length)
{
	bool setOffOk = true;
	if (offset)
	{
		LARGE_INTEGER off;
		LARGE_INTEGER newOff;
		off.QuadPart = (LONGLONG)*offset;
		newOff.QuadPart = 0;
		setOffOk = FALSE != ::SetFilePointerEx(hFile, off, &newOff, FILE_BEGIN) && off.QuadPart == newOff.QuadPart;
	}
	if (setOffOk)
	{
		if (length)
		{
			return true;
		}
		LARGE_INTEGER fileSize;
		if (FALSE != ::GetFileSizeEx(hFile, &fileSize))
		{
			unsigned long long sl = (unsigned long long)fileSize.QuadPart - (offset ? *offset : 0);
			if (0 == (sl >> 32))
			{
				length = (size_t)sl;
				return true;
			}
		}
	}
	else
	{
		length = -1;
	}
	return false;
}
#endif
#endif
//////////////////////////////////////////////////////////////////////////

tcp_acceptor::tcp_acceptor(io_engine& ios)
:_ios(ios), _nonBlocking(false)
#ifdef ENABLE_ASIO_PRE_OP
, _preOption(false)
#endif
{}

tcp_acceptor::~tcp_acceptor()
{
}

tcp_socket::result tcp_acceptor::open(const char* ip, unsigned short port)
{
	if (!_acceptor.has())
	{
		try
		{
			_acceptor.create(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port), false);
			set_internal_non_blocking();
			return tcp_socket::result{ 0, 0, true };
		}
		catch (const boost::system::system_error& se)
		{
			return tcp_socket::result{ 0, se.code().value(), !se.code() };
		}
	}
	return tcp_socket::result{ 0, 0, false };
}

tcp_socket::result tcp_acceptor::open_v4(unsigned short port)
{
	if (!_acceptor.has())
	{
		try
		{
			_acceptor.create(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4(), port), false);
			set_internal_non_blocking();
			return tcp_socket::result{ 0, 0, true };
		}
		catch (const boost::system::system_error& se)
		{
			return tcp_socket::result{ 0, se.code().value(), !se.code() };
		}
	}
	return tcp_socket::result{ 0, 0, false };
}

tcp_socket::result tcp_acceptor::open_v6(unsigned short port)
{
	if (!_acceptor.has())
	{
		try
		{
			_acceptor.create(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v6(), port), false);
			set_internal_non_blocking();
			return tcp_socket::result{ 0, 0, true };
		}
		catch (const boost::system::system_error& se)
		{
			return tcp_socket::result{ 0, se.code().value(), !se.code() };
		}
	}
	return tcp_socket::result{ 0, 0, false };
}

tcp_socket::result tcp_acceptor::close()
{
	if (_acceptor.has())
	{
		boost::system::error_code ec;
		_acceptor->close(ec);
		_acceptor.destroy();
		return tcp_socket::result{ 0, ec.value(), !ec };
	}
	return tcp_socket::result{ 0, 0, false };
}

void tcp_acceptor::pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	_preOption = true;
#endif
}

bool tcp_acceptor::is_pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	return _preOption;
#else
	return false;
#endif
}

tcp_socket::result tcp_acceptor::accept(my_actor* host, tcp_socket& socket)
{
	my_actor::quit_guard qg(host);
	return host->trig<tcp_socket::result>([&](trig_once_notifer<tcp_socket::result>&& h)
	{
		async_accept(socket, std::move(h));
	});
}

tcp_socket::result tcp_acceptor::timed_accept(my_actor* host, int ms, tcp_socket& socket)
{
	bool overtime = false;
	tcp_socket::result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_accept(socket, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res));
	return overtime ? tcp_socket::result{ 0, boost::asio::error::timed_out, false } : res;
}

tcp_socket::result tcp_acceptor::try_accept(tcp_socket& socket)
{
	using namespace boost::asio::detail;
	tcp_socket::result res = { 0, 0, false };
	if (_nonBlocking)
	{
		boost::asio::ip::tcp::endpoint remoteEndpoint;
		std::size_t addrLen = remoteEndpoint.capacity();
		boost::system::error_code ec;
		socket_type newSck = socket_ops::accept(_acceptor->native_handle(), remoteEndpoint.data(), &addrLen, ec);
		if (invalid_socket != newSck)
		{
			if (!ec)
			{
				remoteEndpoint.resize(addrLen);
				socket._socket.assign(remoteEndpoint.protocol(), newSck, ec);
			}
			if (ec)
			{
				res.code = ec.value();
				socket_ops::state_type state = socket_ops::stream_oriented;
				socket_ops::close(newSck, state, true, ec);
			}
			else
			{
				res.ok = true;
				res.s = 1;
			}
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

void tcp_acceptor::set_internal_non_blocking()
{
	boost::system::error_code ec;
	using namespace boost::asio::detail;
	socket_ops::state_type state = socket_ops::user_set_non_blocking;
	_nonBlocking = socket_ops::set_internal_non_blocking(_acceptor->native_handle(), state, true, ec);
}
//////////////////////////////////////////////////////////////////////////

udp_socket::udp_socket(io_engine& ios)
:_socket(ios), _nonBlocking(false)
#ifdef ENABLE_ASIO_PRE_OP
, _preOption(false)
#endif
{}

udp_socket::~udp_socket()
{}

udp_socket::result udp_socket::open_v4()
{
	boost::system::error_code ec;
	_socket.open(boost::asio::ip::udp::v4(), ec);
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::open_v6()
{
	boost::system::error_code ec;
	_socket.open(boost::asio::ip::udp::v6(), ec);
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::bind(const char* ip, unsigned short port)
{
	boost::system::error_code ec;
	_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip), port), ec);
	if (!ec)
	{
		set_internal_non_blocking();
	}
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::bind_v4(unsigned short port)
{
	boost::system::error_code ec;
	_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(), port), ec);
	if (!ec)
	{
		set_internal_non_blocking();
	}
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::bind_v6(unsigned short port)
{
	boost::system::error_code ec;
	_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address_v6(), port), ec);
	if (!ec)
	{
		set_internal_non_blocking();
	}
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::open_bind_v4(unsigned short port)
{
	result res = open_v4();
	if (res.ok)
	{
		return bind_v4(port);
	}
	return res;
}

udp_socket::result udp_socket::open_bind_v6(unsigned short port)
{
	result res = open_v6();
	if (res.ok)
	{
		return bind_v6(port);
	}
	return res;
}

udp_socket::result udp_socket::close()
{
	boost::system::error_code ec;
	_socket.shutdown(boost::asio::ip::udp::socket::shutdown_both, ec);
	_socket.close(ec);
	return result{ 0, ec.value(), !ec };
}

void udp_socket::pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	_preOption = true;
#endif
}

bool udp_socket::is_pre_option()
{
#ifdef ENABLE_ASIO_PRE_OP
	return _preOption;
#else
	return false;
#endif
}

bool udp_socket::try_again(const result& res)
{
	return boost::asio::error::try_again == res.code || boost::asio::error::would_block == res.code;
}

boost::asio::ip::udp::endpoint udp_socket::make_endpoint(const char* remoteIp, unsigned short remotePort)
{
	return boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(remoteIp), remotePort);
}

const boost::asio::ip::udp::endpoint& udp_socket::last_remote_sender_endpoint()
{
	return _remoteSenderEndpoint;
}

void udp_socket::reset_remote_sender_endpoint()
{
	_remoteSenderEndpoint = boost::asio::ip::udp::endpoint();
}

void udp_socket::set_internal_non_blocking()
{
	boost::system::error_code ec;
	using namespace boost::asio::detail;
	socket_ops::state_type state = socket_ops::user_set_non_blocking;
	_nonBlocking = socket_ops::set_internal_non_blocking(_socket.native_handle(), state, true, ec);
}

udp_socket::result udp_socket::connect(const char* remoteIp, unsigned short remotePort)
{
	return connect(make_endpoint(remoteIp, remotePort));
}

udp_socket::result udp_socket::connect(const boost::asio::ip::udp::endpoint& remoteEndpoint)
{
	boost::system::error_code ec;
	_socket.connect(remoteEndpoint, ec);
	if (!ec)
	{
		set_internal_non_blocking();
	}
	return result{ 0, ec.value(), !ec };
}

udp_socket::result udp_socket::send_to(my_actor* host, const boost::asio::ip::udp::endpoint& remoteEndpoint, const void* buff, size_t length, int flags)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_send_to(remoteEndpoint, buff, length, std::move(h), flags);
	});
}

udp_socket::result udp_socket::send_to(my_actor* host, const char* remoteIp, unsigned short remotePort, const void* buff, size_t length, int flags)
{
	return udp_socket::send_to(host, make_endpoint(remoteIp, remotePort), buff, length, flags);
}

udp_socket::result udp_socket::send(my_actor* host, const void* buff, size_t length, int flags)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_send(buff, length, std::move(h), flags);
	});
}

udp_socket::result udp_socket::receive_from(my_actor* host, void* buff, size_t length, int flags)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_receive_from(buff, length, std::move(h), flags);
	});
}

udp_socket::result udp_socket::receive(my_actor* host, void* buff, size_t length, int flags)
{
	my_actor::quit_guard qg(host);
	return host->trig<result>([&](trig_once_notifer<result>&& h)
	{
		async_receive(buff, length, std::move(h), flags);
	});
}

udp_socket::result udp_socket::timed_send_to(my_actor* host, int ms, const boost::asio::ip::udp::endpoint& remoteEndpoint, const void* buff, size_t length, int flags)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_send_to(remoteEndpoint, buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res), flags);
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

udp_socket::result udp_socket::timed_send_to(my_actor* host, int ms, const char* remoteIp, unsigned short remotePort, const void* buff, size_t length, int flags)
{
	return timed_send_to(host, ms, make_endpoint(remoteIp, remotePort), buff, length, flags);
}

udp_socket::result udp_socket::timed_send(my_actor* host, int ms, const void* buff, size_t length, int flags)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_send(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res), flags);
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

udp_socket::result udp_socket::timed_receive_from(my_actor* host, int ms, void* buff, size_t length, int flags)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_receive_from(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res), flags);
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

udp_socket::result udp_socket::timed_receive(my_actor* host, int ms, void* buff, size_t length, int flags)
{
	bool overtime = false;
	result res = { 0, 0, false };
	my_actor::quit_guard qg(host);
	async_receive(buff, length, host->make_asio_timed_context(ms, [&]()
	{
		overtime = true;
		close();
	}, res), flags);
	return overtime ? result{ 0, boost::asio::error::timed_out, false } : res;
}

udp_socket::result udp_socket::try_send(const void* buff, size_t length, int flags)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::send(_socket.native_handle(), &buf, 1, flags, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

udp_socket::result udp_socket::try_msend(const void* const* buffs, const size_t* lengths, size_t count, size_t* bytes, int flags)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::sendmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, flags | MSG_NOSIGNAL);
		if (pcks > 0)
		{
			res.s += pcks;
			if (bytes)
			{
				for (size_t j = 0; j < (size_t)pcks; j++)
				{
					bytes[i*fixed_array_length(msgs) + j] = mhdr[j].msg_len;
				}
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					if (bytes)
					{
						bytes += lastTotPcks;
					}
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_send(buffs[i], lengths[i], flags);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		res.s++;
		if (bytes)
		{
			bytes[i] = tr.s;
		}
	}
	res.ok = true;
	return res;
#endif
}

udp_socket::result udp_socket::try_msend_to(const boost::asio::ip::udp::endpoint* remoteEndpoints, const void* const* buffs, const size_t* lengths, size_t count, size_t* bytes /* = NULL */, int flags /* = 0 */)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
			mhdr[ct].msg_hdr.msg_name = (void*)remoteEndpoints[i*fixed_array_length(msgs) + ct].data();
			mhdr[ct].msg_hdr.msg_namelen = remoteEndpoints[i*fixed_array_length(msgs) + ct].size();
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::sendmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, flags | MSG_NOSIGNAL);
		if (pcks > 0)
		{
			res.s += pcks;
			if (bytes)
			{
				for (size_t j = 0; j < (size_t)pcks; j++)
				{
					bytes[i*fixed_array_length(msgs) + j] = mhdr[j].msg_len;
				}
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					if (bytes)
					{
						bytes += lastTotPcks;
					}
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_send_to(remoteEndpoints[i], buffs[i], lengths[i], flags);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		res.s++;
		if (bytes)
		{
			bytes[i] = tr.s;
		}
	}
	res.ok = true;
	return res;
#endif
}

udp_socket::result udp_socket::try_msend_to(const boost::asio::ip::udp::endpoint& remoteEndpoint, const void* const* buffs, const size_t* lengths, size_t count, size_t* bytes /* = NULL */, int flags /* = 0 */)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
			mhdr[ct].msg_hdr.msg_name = (void*)remoteEndpoint.data();
			mhdr[ct].msg_hdr.msg_namelen = remoteEndpoint.size();
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::sendmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, flags | MSG_NOSIGNAL);
		if (pcks > 0)
		{
			res.s += pcks;
			if (bytes)
			{
				for (size_t j = 0; j < (size_t)pcks; j++)
				{
					bytes[i*fixed_array_length(msgs) + j] = mhdr[j].msg_len;
				}
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					if (bytes)
					{
						bytes += lastTotPcks;
					}
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_send_to(remoteEndpoint, buffs[i], lengths[i], flags);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		res.s++;
		if (bytes)
		{
			bytes[i] = tr.s;
		}
	}
	res.ok = true;
	return res;
#endif
}

udp_socket::result udp_socket::try_send_to(const char* remoteIp, unsigned short remotePort, const void* buff, size_t length, int flags)
{
	return try_send_to(make_endpoint(remoteIp, remotePort), buff, length, flags);
}

udp_socket::result udp_socket::try_send_to(const boost::asio::ip::udp::endpoint& remoteEndpoint, const void* buff, size_t length, int flags)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::sendto(_socket.native_handle(), &buf, 1, flags, remoteEndpoint.data(), remoteEndpoint.size(), ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

udp_socket::result udp_socket::try_receive(void* buff, size_t length, int flags)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::recv(_socket.native_handle(), &buf, 1, flags, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}

udp_socket::result udp_socket::try_mreceive(void* const* buffs, const size_t* lengths, size_t count, size_t* bytes, int flags)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::recvmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, flags, NULL);
		if (pcks > 0)
		{
			res.s += pcks;
			if (bytes)
			{
				for (size_t j = 0; j < (size_t)pcks; j++)
				{
					bytes[i*fixed_array_length(msgs) + j] = mhdr[j].msg_len;
				}
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					if (bytes)
					{
						bytes += lastTotPcks;
					}
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_receive(buffs[i], lengths[i], flags);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		res.s++;
		if (bytes)
		{
			bytes[i] = tr.s;
		}
	}
	res.ok = true;
	return res;
#endif
}

udp_socket::result udp_socket::try_mreceive_from(boost::asio::ip::udp::endpoint* remoteEndpoints, void* const* buffs, const size_t* lengths, size_t count, size_t* bytes /* = NULL */, int flags /* = 0 */)
{
#ifdef ENABLE_SCK_MULTI_IO
	result res = { 0, 0, false };
	size_t i = 0;
	while (true)
	{
		struct iovec msgs[32];
		struct mmsghdr mhdr[32];
		size_t ct = 0;
		for (; ct < fixed_array_length(msgs); ct++)
		{
			size_t k = i*fixed_array_length(msgs) + ct;
			if (k >= count)
			{
				break;
			}
			msgs[ct].iov_base = (void*)buffs[k];
			msgs[ct].iov_len = lengths[k];
			memset(&mhdr[ct], 0, sizeof(mhdr[ct]));
			mhdr[ct].msg_hdr.msg_iov = &msgs[ct];
			mhdr[ct].msg_hdr.msg_iovlen = 1;
			mhdr[ct].msg_hdr.msg_name = (void*)remoteEndpoints[i*fixed_array_length(msgs) + ct].data();
			mhdr[ct].msg_hdr.msg_namelen = remoteEndpoints[i*fixed_array_length(msgs) + ct].capacity();
		}
		if (!ct)
		{
			break;
		}
		const int pcks = ::recvmmsg(_socket.native_handle(), mhdr, (unsigned int)ct, flags, NULL);
		if (pcks > 0)
		{
			res.s += pcks;
			for (size_t j = 0; j < (size_t)pcks; j++)
			{
				remoteEndpoints[i*fixed_array_length(msgs) + j].resize(mhdr[ct].msg_hdr.msg_namelen);
			}
			if (bytes)
			{
				for (size_t j = 0; j < (size_t)pcks; j++)
				{
					bytes[i*fixed_array_length(msgs) + j] = mhdr[j].msg_len;
				}
			}
			if ((size_t)pcks != ct)
			{
				if (EINTR == errno)
				{
					const size_t lastTotPcks = i*fixed_array_length(msgs) + pcks;
					i = 0;
					buffs += lastTotPcks;
					lengths += lastTotPcks;
					count -= lastTotPcks;
					if (bytes)
					{
						bytes += lastTotPcks;
					}
					continue;
				}
				break;
			}
		}
		else
		{
			const int err = errno;
			if (EINTR == err)
			{
				continue;
			}
			if (res.s && (EAGAIN == err || EWOULDBLOCK == err))
			{
				break;
			}
			res.code = err;
			return res;
		}
		i++;
	}
	res.ok = true;
	return res;
#else
	result res = { 0, 0, false };
	for (size_t i = 0; i < count; i++)
	{
		result tr = try_receive_from(remoteEndpoints[i], buffs[i], lengths[i], flags);
		if (!tr.ok)
		{
			if (i && try_again(tr))
			{
				break;
			}
			res.code = tr.code;
			return res;
		}
		res.s++;
		if (bytes)
		{
			bytes[i] = tr.s;
		}
	}
	res.ok = true;
	return res;
#endif
}

udp_socket::result udp_socket::try_receive_from(void* buff, size_t length, int flags)
{
	return try_receive_from(_remoteSenderEndpoint, buff, length, flags);
}

udp_socket::result udp_socket::try_receive_from(boost::asio::ip::udp::endpoint& remoteEndpoint, void* buff, size_t length, int flags /*= 0*/)
{
	using namespace boost::asio::detail;
	result res = { 0, 0, false };
	if (_nonBlocking)
	{
		socket_ops::buf buf;
		socket_ops::init_buf(buf, buff, length);
		std::size_t addlen = remoteEndpoint.capacity();
		boost::system::error_code ec;
		signed_size_type bytes = socket_ops::recvfrom(_socket.native_handle(), &buf, 1, flags, remoteEndpoint.data(), &addlen, ec);
		if (bytes >= 0 && !ec)
		{
			res.ok = true;
			res.s = (size_t)bytes;
			remoteEndpoint.resize(addlen);
		}
		else
		{
			res.code = ec.value();
		}
	}
	else
	{
		res.code = boost::asio::error::would_block;
	}
	return res;
}
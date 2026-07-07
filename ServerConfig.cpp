#include "Location.hpp"
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : host("localhost"), port(8080), max_body_size(1024 * 1024) {}


ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: host(other.host), port(other.port), max_body_size(other.max_body_size),
	  error_pages(other.error_pages), locations(other.locations) {}
ServerConfig ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		host = other.host;
		port = other.port;
		max_body_size = other.max_body_size;
		error_pages = other.error_pages;
		locations = other.locations;
	}
	return *this;
}

void ServerConfig::setPort(int p)
{
	port = p;
}

void ServerConfig::addLocation(const Location &loc)
{
	locations.push_back(loc);
}

int ServerConfig::getPort() const
{
	return port;
}

const std::vector<Location> &ServerConfig::getLocations() const
{
	return locations;
}

void ServerConfig::setHost(const std::string &h)
{
	host = h;
}

void ServerConfig::setMaxBodySize(size_t size)
{
	max_body_size = size;
}

const std::string &ServerConfig::getHost() const
{
	return host;
}
size_t ServerConfig::getMaxBodySize() const
{
	return max_body_size;
}


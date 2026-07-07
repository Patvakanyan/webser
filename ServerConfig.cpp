#include "Location.hpp"
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : port(8080), max_body_size(1024 * 1024) {}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: server_names(other.server_names), port(other.port), max_body_size(other.max_body_size),
	  error_pages(other.error_pages), locations(other.locations), root(other.root) {}
ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		server_names = other.server_names;
		port = other.port;
		max_body_size = other.max_body_size;
		error_pages = other.error_pages;
		locations = other.locations;
		root = other.root;
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

void ServerConfig::setServerName(const std::string &h)
{
	ServerName server_name;
	server_name.name = h;
	server_name.type = EXACT;
	server_names.push_back(server_name);
	std::cout << "Server name set to: " << h << std::endl;
}
std::vector<ServerName> ServerConfig::getServerNames() const
{
	return server_names;
}
void ServerConfig::setMaxBodySize(size_t size)
{
	max_body_size = size;
}

size_t ServerConfig::getMaxBodySize() const
{
	return max_body_size;
}

void ServerConfig::setRoot(const std::string &r)
{
	root = r;
}

const std::string &ServerConfig::getRoot() const
{
	return root;
}

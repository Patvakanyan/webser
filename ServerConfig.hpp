#pragma once
#include "Location.hpp"
#include "ServerName.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

class ServerConfig
{
private:
	std::vector<ServerName> server_names;
	int port;
	size_t max_body_size;
	std::map<int, std::string> error_pages;
	std::vector<Location> locations;
	std::string root;

public:
	ServerConfig();
	~ServerConfig();
	ServerConfig(const ServerConfig &other);
	ServerConfig &operator=(const ServerConfig &other);

	void setServerName(const std::string &h);
	void setMaxBodySize(size_t size);
	void setPort(int p);
	void setRoot(const std::string &r);
	void addLocation(const Location &loc);

	// Геттеры (для Роли 1 и Роли 3)
	std::vector<ServerName> getServerNames() const;
	int getPort() const;
	const std::string &getRoot() const;
	size_t getMaxBodySize() const;
	const std::vector<Location> &getLocations() const;
};

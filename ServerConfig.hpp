#pragma once

#include "Location.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

class ServerConfig
{
private:
	std::string host;
	int port;
	size_t max_body_size;
	std::map<int, std::string> error_pages;
	std::vector<Location> locations;

public:
	ServerConfig();
	~ServerConfig();
	ServerConfig(const ServerConfig &other);
	ServerConfig operator=(const ServerConfig &other);

	void setHost(const std::string &h);
	void setMaxBodySize(size_t size);
	void setPort(int p);
	void addLocation(const Location &loc);

	// Геттеры (для Роли 1 и Роли 3)
	int getPort() const;
	const std::string &getHost() const;
	size_t getMaxBodySize() const;
	const std::vector<Location> &getLocations() const;
};

#include "Location.hpp"
#include "ServerConfig.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <vector>

std::vector<ServerConfig> validation_config(const std::vector<std::string> &tokens)
{
	bool in_server = false;
	bool in_location = false;
	ServerConfig currentServer;
	Location currentLocation;
	std::vector<ServerConfig> servers;

	for (size_t i = 0; i < tokens.size(); i++)
	{
		const std::string &token = tokens[i];
		if (token == "}")
		{
			if (in_location)
			{
				in_location = false;
				currentServer.addLocation(currentLocation);
				currentLocation = Location();
			}
			else if (in_server)
			{
				in_server = false;
				servers.push_back(currentServer);
				currentServer = ServerConfig();
			}
			else
			{
				throw std::runtime_error("Syntax error: unexpected '}'");
			}
			continue;
		}
		if (token == "server")
		{
			if (in_server)
				throw std::runtime_error("Error: nested servers not allowed");
			if (i + 1 >= tokens.size() || tokens[++i] != "{")
				throw std::runtime_error("Syntax error: expected '{' after 'server'");
			in_server = true;
			continue;
		}
		if (token == "location")
		{
			if (!in_server || in_location)
				throw std::runtime_error("Error: location must be inside server");
			if (i + 1 >= tokens.size() || tokens[i + 1][0] != '/')
				throw std::runtime_error("Syntax error: expected path starting with '/' after 'location'");

			currentLocation.setPath(tokens[++i]);

			if (i + 1 >= tokens.size() || tokens[++i] != "{")
				throw std::runtime_error("Syntax error: expected '{' after location path");
			in_location = true;
			continue;
		}
		if (in_location)
		{
			if (token == "method" || token == "methods")
			{
				while (++i < tokens.size() && tokens[i] != ";")
				{
					currentLocation.addMethod(tokens[i]);
				}
				if (i >= tokens.size() || tokens[i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after method");
			}
			else if (token == "root")
			{
				if (i + 1 >= tokens.size())
					throw std::runtime_error("Error: missing value for root");
				currentLocation.setRoot(tokens[++i]);
				if (i + 1 >= tokens.size() || tokens[++i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after root");
			}
			else if (token == "index")
			{
				if (i + 1 >= tokens.size())
					throw std::runtime_error("Error: missing value for index");
				currentLocation.setIndex(tokens[++i]);
				if (i + 1 >= tokens.size() || tokens[++i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after index");
			}
			else
				throw std::runtime_error("Unknown directive in location block: " + token);
		}
		else if (in_server)
		{
			if (token == "listen")
			{
				if (i + 1 >= tokens.size())
					throw std::runtime_error("Error: Missing port number after 'listen'.");
				std::string portStr = tokens[++i];

				if (portStr == ";")
					throw std::runtime_error("Error: listen directive has no value!");

				char *endptr;
				errno = 0;
				long portNum = std::strtol(portStr.c_str(), &endptr, 10);

				if (errno == ERANGE)
					throw std::runtime_error("Error: Port number is too large.");
				if (portStr.c_str() == endptr || *endptr != '\0')
					throw std::runtime_error("Error: Invalid characters in port number.");
				if (portNum < 1 || portNum > 65535)
					throw std::runtime_error("Error: Port out of valid range (1 - 65535).");

				currentServer.setPort(static_cast<int>(portNum));

				if (i + 1 >= tokens.size() || tokens[++i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after listen");
			}
			else if (token == "server_name")
			{
				if (i + 1 >= tokens.size())
					throw std::runtime_error("Error: missing value for server_name");
				currentServer.setHost(tokens[++i]);
				if (i + 1 >= tokens.size() || tokens[++i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after server_name");
			}
			else if (token == "max_body_size")
			{
				if (i + 1 >= tokens.size())
					throw std::runtime_error("Error: Missing value after 'max_body_size'.");
				std::string sizeStr = tokens[++i];

				if (sizeStr == ";")
					throw std::runtime_error("Error: max_body_size directive has no value!");
				char *endptr;
				errno = 0;
				long sizeNum = std::strtol(sizeStr.c_str(), &endptr, 10);

				if (errno == ERANGE)
					throw std::runtime_error("Error: Size number is too large.");
				if (sizeStr.c_str() == endptr || *endptr != '\0')
					throw std::runtime_error("Error: Invalid characters in body size.");
				if (sizeNum < 1 || sizeNum > 1024 * 1024 * 1024)
					throw std::runtime_error("Error: Size out of valid range.");

				currentServer.setMaxBodySize(static_cast<size_t>(sizeNum));

				if (i + 1 >= tokens.size() || tokens[++i] != ";")
					throw std::runtime_error("Syntax error: expected ';' after max_body_size");
			}
			else
				throw std::runtime_error("Unknown directive in server block: " + token);
		}
		else
			throw std::runtime_error("Unknown token outside of blocks: " + token);
	}
	if (in_server || in_location)
		throw std::runtime_error("Syntax error: missing closing '}'");
	return servers;
}

void parse_file(const std::string &filename)
{
	std::ifstream configFile(filename.c_str());
	if (!configFile.is_open())
		throw std::runtime_error("Failed to open config file");

	std::string line;
	std::vector<std::string> tokens;

	while (std::getline(configFile, line))
	{
		size_t commentPos = line.find("#");
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		std::string sLine;
		for (size_t i = 0; i < line.size(); i++)
		{
			char c = line[i];
			if (c == '{' || c == '}' || c == ';')
			{
				sLine += " ";
				sLine += c;
				sLine += " ";
			}
			else
				sLine += c;
		}

		std::stringstream ss(sLine);
		std::string word;
		while (ss >> word)
			tokens.push_back(word);
	}
	configFile.close();

	std::vector<ServerConfig> servers = validation_config(tokens);

	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << "Server " << i + 1 << ":" << std::endl;
		std::cout << "  Host: " << servers[i].getHost() << std::endl;
		std::cout << "  Port: " << servers[i].getPort() << std::endl;
		std::cout << "  Max Body Size: " << servers[i].getMaxBodySize() << std::endl;
		const std::vector<Location> &locations = servers[i].getLocations();
		for (size_t j = 0; j < locations.size(); ++j)
		{
			std::cout << "    Location " << j + 1 << ":" << std::endl;
			std::cout << "      Path: " << locations[j].getPath() << std::endl;
			std::cout << "      Root: " << locations[j].getRoot() << std::endl;
			const std::vector<std::string> &methods = locations[j].getMethods();
			for (size_t k = 0; k < methods.size(); ++k)
				std::cout << "      Method: " << methods[k] << std::endl;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	try
	{
		parse_file(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr  << e.what() << std::endl;
		return 1;
	}
	return 0;
}

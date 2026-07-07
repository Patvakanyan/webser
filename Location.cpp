#include "Location.hpp"

Location::Location() : root(""), index("index.html") {}

Location::~Location() {}

Location::Location(const Location &other)
	: path(other.path), methods(other.methods), root(other.root),
	  index(other.index), autoindex(other.autoindex), redirection(other.redirection) {}

Location Location::operator=(const Location &other)
{
	if (this != &other)
	{
		path = other.path;
		methods = other.methods;
		root = other.root;
		index = other.index;
		autoindex = other.autoindex;
		redirection = other.redirection;
	}
	return *this;
}

void Location::setPath(const std::string &p)
{
	path = p;
}
void Location::addMethod(const std::string &m)
{
	methods.push_back(m);
}

void Location::setRoot(const std::string &r)
{
	root = r;
}

std::string Location::getPath() const
{
	return path;
}

const std::vector<std::string> &Location::getMethods() const
{
	return methods;
}

std::string Location::getRoot() const
{
	return root;
}

void Location::setIndex(const std::string &i)
{
	index = i;
}
void Location::setAutoindex(bool a)
{
	autoindex = a;
}

void Location::setRedirection(const std::string &r)
{
	redirection = r;
}


bool Location::getAutoindex() const
{
	return autoindex;
}

std::string Location::getIndex() const
{
	return index;
}
std::string Location::getRedirection() const
{
	return redirection;
}

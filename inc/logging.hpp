#ifndef LOGGING_HEADER_
#define LOGGING_HEADER_
#include <iostream>
#include <fstream>
template <typename T>
void log(std::ostream &stream, const T & obj)
{
	stream << obj << std::endl;
}

template <typename T, typename... agcs>
void log (std::ostream &stream, const T& obj, const agcs&... arg)
{
	stream << obj;
	log(stream, arg...);
}

#ifdef D_DEBUG

static std::ofstream logger("logfile.txt", std::ios::app);

#define printLog(...) log(logger,"Message::Fille [", __FILE__, "] fuction[", __FUNCTION__, "] line[", __LINE__, "] message:[", __VA_ARGS__, "]")
#define printError(...) log(logger,"Error::Fille [", __FILE__, "] fuction[", __FUNCTION__, "] line[", __LINE__, "] message:[", __VA_ARGS__, "]")
#define printWarning(...) log(logger,"Warning::Fille [", __FILE__, "] fuction[", __FUNCTION__, "] line[", __LINE__, "] message:[", __VA_ARGS__, "]")

#else
#define printLog(...) 
#define printWarning(...)
#define printError(...)
#endif

#endif // LOGGING_HEADER_

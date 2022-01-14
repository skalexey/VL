#include "JSONConverter.h"
#include "JSONConverter.h"
#include "JSONWriter.h"
#include "JSONLoader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <sstream>

using namespace rapidjson;

bool vl::JSONConverter::Store(vl::Object& object, const vl::Object& context, const char* filePath, const CnvParams& params)
{
	std::string jsonStr;
	if (!JSONStr(object, context, jsonStr, params))
		return false;
	std::ofstream f;
	f.open(filePath);
	f << jsonStr;
	f.close();

	return true;
}

bool vl::JSONConverter::JSONStr(vl::Object& object, const vl::Object& context, std::string& out, const CnvParams& params)
{
	vl::JSONWriter v(context, params);
	object.Accept(v);
	
	Document& d = v.GetDocument();

	StringBuffer buffer;

	if (params.pretty)
	{
		PrettyWriter<StringBuffer> writer(buffer);
		if (!d.Accept(writer))
			return false;
	}
	else
	{
		Writer<StringBuffer> writer(buffer);
		if (!d.Accept(writer))
			return false;
	}
	out = buffer.GetString();
	return true;
}

std::string vl::JSONConverter::JSONStr(Object& object, const vl::Object& context, const CnvParams& params)
{
	std::string s;
	JSONStr(object, context, s, params);
	return s;
}

bool vl::JSONConverter::Load(Object& object, const char* filePath)
{
	std::ifstream f;
	f.open(filePath);
	std::stringstream buf;
	buf << f.rdbuf();
	f.close();
	rapidjson::Document d;
	d.Parse<0>(buf.str().c_str());
	vl::JSONLoader loader(object);
	return d.Accept(loader);
}


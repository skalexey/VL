#include <iostream>
#include "vl.h"

void CreateVariableTest()
{
	auto s = vl::String("Hello");
	std::cout << s.Val() << "\n";

	auto n = vl::Number(321.4);
	std::cout << n.Val() << "\n";

	auto b = vl::Bool(true);
	std::cout << (b.Val() ? "true" : "false") << "\n";
}

vl::Object CreateObjectTest()
{
	// Create an object with some fields
	auto pear = vl::Object();
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);

	// Print the content
	std::cout << "Object content:\n";
	pear.ForeachProp([&](auto& k, auto& v) {
		std::cout << "	[" << k << "]: " << v.ToStr() << "\n";
		return true;
	});
	return pear;
}

void CreateListTest(vl::Object& pear)
{
	// Create a list with differently-typed elements
	auto list = vl::List();
	list.Add(pear);
	list.Add(vl::String("A text"));
	list.Add(vl::Bool(true));
	list.Add(vl::Number(41));

	// Print the content
	std::cout << "\nList content:\n";
	for (int i = 0; i < list.Size(); i++)
		std::cout << "	[" << i << "]: " << list.At(i).ToStr() << "\n";
}

void SetProtoTest()
{
	auto fruit = vl::Object();
	fruit.Set("isFruit", true);
	fruit.Set("title", "Fruit");
	fruit.Set("color", "Green");
	fruit.Set("radius", 0.2f);
	fruit.Set("branchCount", 0);

	auto pear = vl::Object();
	pear.SetPrototype(fruit);
	pear.Set("title", "Pear");
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	pear.Set("isSweet", true);

	// Print the content
	std::cout << pear.Get("title").as<vl::String>().Val() << " properties:\n";
	pear.ForeachProp([&](auto& k, auto& v) {
		std::cout << "	[" << k << "]: " << v.ToStr() << "\n";
		return true;
	}, true);
}

int main(int argc, const char* argv[])
{
	std::cout << "VL Project Test Unit\n";
	
	CreateVariableTest();
	auto o = CreateObjectTest();
	CreateListTest(o);
	SetProtoTest();

	return 0;
}

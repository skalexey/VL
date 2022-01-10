#include <iostream>
#include "vl.h"

int main(int argc, const char* argv[])
{
	std::cout << "VL Project Test Unit\n";
	auto pear = vl::Object();
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	std::cout << "Val: " << pear.Get("color").AsString().Val() << "\n";
	return 0;
}

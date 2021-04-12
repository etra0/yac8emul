#include <doctest/doctest.h>
#include <yac8emul/interpreter.h>

TEST_CASE("sum") {
	CHECK(sum(2, 2) == 4);
}

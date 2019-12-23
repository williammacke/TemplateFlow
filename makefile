src := TEMPLATEFLOW/*
tests := tests/*
LDFLAGS := -lgtest -lgtest_main -pthread
CPPFLAGS := -std=c++17 -Wall -I TEMPLATEFLOW -I gcem
CC := g++
test_output := TEST_TEMPLATE_FLOW

test: $(src) $(tests)
	$(CC) $(CPPFLAGS) $(tests) $(LDFLAGS) -o $(test_output)
	./$(test_output)

COMPILE_CMD = g++ -std=c++17 -I src -I rapidjson/include src/JsonSerdeStatics.cpp 

build/basic_test: test/basic_test.cpp
	mkdir -p build
	${COMPILE_CMD}  test/basic_test.cpp -o build/basic_test
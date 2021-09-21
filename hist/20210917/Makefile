.PHONY: test run-test

test:
	@cp -rf -t include/lspe src/*
	@g++ -std=c++1z -Iinclude -o build/testunit test/testunit.cpp
run-test: test
	@build/testunit
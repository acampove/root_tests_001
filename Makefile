all: test

test: test.cxx
	g++ test.cxx -o test `root-config --glibs --cflags` -lROOTDataFrame
clean:
	rm -f test


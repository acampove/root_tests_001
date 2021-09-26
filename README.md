## Build

In a machine with ROOT setup do:

```
make
```

to get `test`

## Run

```
./test 100000 1
```

will:

1. Create two files `file_1.root` and `file_2.root`.
2. Load them, add a branch with `RDataFrame` 
3. Save them.
4. Load them again.
5. Merge them and save the merged file.

```
./test 100000 2
```

will:

1. Create two files `file_1.root` and `file_2.root`.
2. Load them in a chain.
3. Clone the chain.
4. Add a branch.
5. Loop over the chain adding entries to clone.
6. Save.

Both methods will be timed. Results in my machine:
```
 23:47:04 campoverde@lxslc708 ./test 100000 1
Time: 5.30999994 sec 
 23:47:57 campoverde@lxslc708 ./test 100000 2
Time: 0.28999999 sec 
```


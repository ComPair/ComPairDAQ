# ComPair DAQ

## Build Instructions
```
mkdir build
cmake ..
make -j install
```

## Running Tests
Tests must be enabled with the a CMake option and then can be run via a number
of methods. The simplest prescription is as follows:
```
cd build
cmake .. -Dtests=ON
make -j test
```

More verbose output can be found by running `ctest`:
```
ctest --verbose
```

## Documentation
Code docuemntation can be genreated with doxygen.
```
doxygen doc/Doxyfile
```
One can include the tests as well by using `Doxyfile_wTests`. The output is
generated in `doc/html`.


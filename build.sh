# simple script to build NMage
rm build -rf
mkdir build
cd build
cmake ..
make
cp nmage ../

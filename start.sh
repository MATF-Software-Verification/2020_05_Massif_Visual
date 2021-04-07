mkdir build
cd build/

app="MassifVisualizer"

qmake ../MassifVisualizer/"$app".pro
  
make
rm *.o

echo "Build done!"
echo "$app is starting now..."
./MassifVisualizer

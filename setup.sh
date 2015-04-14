#echo "Setting up allegro..."
#sudo apt-get update
#sudo apt-get install build-essential git cmake cmake-curses-gui xorg-dev libgl1-mesa-dev libglu-dev
#sudo apt-get install libpng-dev libcurl4-openssl-dev libfreetype6-dev libjpeg-dev libvorbis-dev libopenal-dev libphysfs-dev \
# libgtk2.0-dev libasound-dev libpulse-dev libflac-dev libdumb1-dev
#git clone http://git.code.sf.net/p/alleg/allegro
#cd allegro
#git checkout 5.0
#mkdir build 
#cd build
#ccmake -DCMAKE_INSTALL_PREFIX=/usr ..
#make
#make install
#cd ..
#cd ..
echo "Setting up files..."
echo "Setting up Java server..."
javac GreetingServer.java
echo "Setting up solution finders..."
g++ -o image_processed_cube_solver image_processed_cube_solver.cpp
g++ -o alogrithm_finder algorithm_finder.cpp 
g++ -o analysis analysis.cpp
echo "Setting up display utilities..."
g++ -o pyraminx_shuffler pyraminx_shuffler.cpp -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
g++ -o solution_display solution_display.cpp -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
g++ -o algorithm_finder_gui algorithm_finder_gui.cpp -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
echo "Lubricating the Pyraminx with Japanese oil..."
sleep 1;
echo "Buying Lays chips for the evening..."
sleep 1;
echo "Just wasting some time..."
sleep 1;
echo "Done :) "
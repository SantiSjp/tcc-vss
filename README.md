## TCC VSSS ##


Very Small Size Soccer TCC Project


## INSTALLATION ##

>> Run InstallDependencies as sudo 
	sudo ./InstallDependencies

>> Clone opencv in a new directory (ex.: /tmp): 
	git clone https://github.com/opencv/opencv.git

>> Install opencv
	cd ~/opencv
	mkdir build
	cd build
	cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
	sudo make install
	

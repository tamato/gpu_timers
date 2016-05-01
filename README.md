### Clone this project with the --recursive flag  

###Building  
It is expected to do an out of source build  
To get this project up and running it has been tested in Windows 10 with Visual Studio 12 2013 with the command:  
cmake -G "Visual Studio 12 (2013)" "64" ../boids  

And in Ubuntu 14.10 with gcc 4.8.4 and clang 3.7.1 with the commands  
for gcc  
cmake ../boids  
cmake -G "Sublime Text 2 - Unix Makefiles" ../boids  
cmake -G "Sublime Text 2 - Ninja" ../boids  

for clang  
CC=clang CXX=clang++ cmake ../boids  
CC=clang CXX=clang++ cmake -G "Sublime Text 2 - Unix Makefiles" ../boids  
CC=clang CXX=clang++ cmake -G "Sublime Text 2 - Ninja" ../boids  

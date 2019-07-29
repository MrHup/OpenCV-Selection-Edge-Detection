g++ -std=c++11 -o output main.cpp InterestZone.cpp menu.cpp display.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql `pkg-config --cflags --libs opencv`

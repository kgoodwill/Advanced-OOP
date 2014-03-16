CPPFLAGS = -g -Wall -Wextra -pedantic -std=c++0x

all: ken chaoren minimal morse

ken:
	g++ $(CPPFLAGS) test_ken.cpp -o test_ken.exe

chaoren:
	g++ $(CPPFLAGS) test_chaoren.cpp -o test_chaoren.exe

minimal:
	g++ $(CPPFLAGS) minimal.cpp -o minimal.exe

morse:
	g++ $(CPPFLAGS) morseex.cpp -o morse.exe

clean:
	rm *.exe*

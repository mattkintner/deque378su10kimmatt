cs:
	@echo `pwd`
	@rsync -r -t -u -v --delete \
	--include "makefile"        \
	--include "*.c++"           \
	--include "*.h"             \
	--include "*.html"          \
	--include "*.java"          \
	--include "*.out"           \
	--exclude "*"               \
	. downing@$(CS):cs/projects/deque/

OS       := $(shell uname)
VALGRIND := $(shell which valgrind)

ifeq ($(OS), Darwin)
	BOOST :=
else
	BOOST := -I/public/linux/include/boost-1_38
endif

.PRECIOUS: %.c++.app
.PRECIOUS: %.class

TestDeque.c++.app: TestDeque.c++ Deque.h
	g++ -ansi -pedantic $(BOOST) -lcppunit -ldl -Wall $< -o TestDeque.c++.app

TestDeque.class: TestDeque.java Deque.java
	javac -Xlint TestDeque.java

TestDeque.c++x: TestDeque.c++.app
	$(VALGRIND) TestDeque.c++.app

TestDeque.javax: TestDeque.class
	java -ea TestDeque

TestDeque.pyx:
	TestDeque.py

clean:
	rm -f *.app
	rm -f *.class
	rm -f *.pyc

list:
	ls -al *.app
	ls -al *.class

test:
	make TestDeque.c++x
	make TestDeque.javax
	make TestDeque.pyx

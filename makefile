CC=g++ -g
IDIR=-IC:\Users\tomst\Desktop\sfml\SFML-2.5.1\include
LDIR=-LC:\Users\tomst\Desktop\sfml\SFML-2.5.1\lib
LIB=-lsfml-graphics -lsfml-window -lsfml-system
RM=del -r -f
snake.exe:snake.cpp
	$(CC) snake.cpp $(IDIR) $(LDIR) $(LIB) -o snake.exe
clean:
	$(RM) snake.exe
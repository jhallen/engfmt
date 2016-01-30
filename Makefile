OBJS = engfmt.o main.o

engfmt: $(OBJS)
	$(CC) -o engfmt $(OBJS)

clean:
	rm -f $(OBJS)

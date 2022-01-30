CC			= gcc
CFLAGS	= -Wall -g
LDFLAGS	= -lSDL2 -lGLU -lGL -lm -lglut -lSDL_image 

BINDIR	= bin
INCDIR = -I include
SRCDIR	= src
OBJDIR	= obj



SRC_FILES = $(shell find $(SRCDIR)/ -type f -name '*.c' )
OBJ_FILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o, $(SRC_FILES))
EXEC_BIN = terrimac.out


all : $(OBJ_FILES)

terrimac : $(OBJ_FILES)
	@mkdir -p $(BINDIR)/
	$(CC) -o $(BINDIR)/$(EXEC_BIN) $(OBJ_FILES) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) $(INCDIR)



clean :
	rm -rf *~
	rm -rf $(SRCDIR)/*/*~
	rm -rf $(OBJDIR)/
	rm -rf $(BINDIR)/*

